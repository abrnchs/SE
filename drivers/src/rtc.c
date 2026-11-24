#include "rtc.h"
#include "LPC17xx.h"
#include <stddef.h>
#include <stdint.h>

#define RTC_CLOCK_ENABLE     0x01U
#define RTC_DIVIDER_RESET    0x02U
#define RTC_ALARM_FLAG       0x02U
#define RTC_ALL_FLAGS       0x03U
#define RTC_ALARM_DISABLED   0xFFU
/* Comparar minuto, hora, dia do mes, mes e ano; ignorar segundo, DOW e DOY. */
#define RTC_ALARM_FIELDS     0x31U
#define SECONDS_PER_DAY      86400ULL

static volatile bool alarm_triggered = false;
static bool alarm_programmed = false;

static bool RTC_IsLeapYear(uint32_t year)
{
    return (year % 4U == 0U) &&
           ((year % 100U != 0U) || (year % 400U == 0U));
}

static uint8_t RTC_DaysInMonth(uint32_t year, uint32_t month)
{
    static const uint8_t days[12] = {
        31U, 28U, 31U, 30U, 31U, 30U,
        31U, 31U, 30U, 31U, 30U, 31U
    };

    if (month == 2U && RTC_IsLeapYear(year)) {
        return 29U;
    }
    return days[month - 1U];
}

/* Valida a data e calcula os dias decorridos desde 1970-01-01. */
static bool RTC_DaysFromDate(const struct tm *dateTime,
                             uint64_t *daysSinceEpoch,
                             uint16_t *dayOfYear)
{
    int64_t year;
    uint32_t month;
    uint64_t days = 0ULL;
    uint16_t yday = 0U;

    if (dateTime == NULL || daysSinceEpoch == NULL || dayOfYear == NULL) {
        return false;
    }

    year = (int64_t)dateTime->tm_year + 1900LL;
    if (year < 1970LL || year > 4095LL ||
        dateTime->tm_mon < 0 || dateTime->tm_mon > 11 ||
        dateTime->tm_hour < 0 || dateTime->tm_hour > 23 ||
        dateTime->tm_min < 0 || dateTime->tm_min > 59 ||
        dateTime->tm_sec < 0 || dateTime->tm_sec > 59) {
        return false;
    }

    month = (uint32_t)dateTime->tm_mon + 1U;
    if (dateTime->tm_mday < 1 ||
        dateTime->tm_mday > (int)RTC_DaysInMonth((uint32_t)year, month)) {
        return false;
    }

    for (uint32_t current = 1970U; current < (uint32_t)year; current++) {
        days += RTC_IsLeapYear(current) ? 366ULL : 365ULL;
    }
    for (uint32_t current = 1U; current < month; current++) {
        yday = (uint16_t)(yday + RTC_DaysInMonth((uint32_t)year, current));
    }
    yday = (uint16_t)(yday + (uint16_t)dateTime->tm_mday);
    days += (uint64_t)(yday - 1U);

    *daysSinceEpoch = days;
    *dayOfYear = yday;
    return true;
}

static bool RTC_TimeDateFromSeconds(time_t seconds, struct tm *dateTime)
{
    uint64_t value;
    uint64_t days;
    uint64_t secondsToday;
    uint64_t daysSinceEpoch;
    uint32_t year = 1970U;
    uint32_t month = 1U;
    uint32_t daysThisYear;
    uint32_t daysThisMonth;
    int dayOfYear;

    if (dateTime == NULL) {
        return false;
    }

    value = (uint64_t)seconds;
    daysSinceEpoch = value / SECONDS_PER_DAY;
    days = daysSinceEpoch;
    secondsToday = value % SECONDS_PER_DAY;

    while (year <= 4095U) {
        daysThisYear = RTC_IsLeapYear(year) ? 366U : 365U;
        if (days < daysThisYear) {
            break;
        }
        days -= daysThisYear;
        year++;
    }
    if (year > 4095U) {
        return false;
    }

    dayOfYear = (int)days;
    while (month <= 12U) {
        daysThisMonth = RTC_DaysInMonth(year, month);
        if (days < daysThisMonth) {
            break;
        }
        days -= daysThisMonth;
        month++;
    }

    dateTime->tm_year = (int)year - 1900;
    dateTime->tm_mon = (int)month - 1;
    dateTime->tm_mday = (int)days + 1;
    dateTime->tm_hour = (int)(secondsToday / 3600ULL);
    dateTime->tm_min = (int)((secondsToday % 3600ULL) / 60ULL);
    dateTime->tm_sec = (int)(secondsToday % 60ULL);
    dateTime->tm_wday = (int)((daysSinceEpoch + 4ULL) % 7ULL);
    dateTime->tm_yday = dayOfYear;
    dateTime->tm_isdst = 0;
    return true;
}

void RTC_Init(time_t seconds)
{
    struct tm dateTime;

    LPC_SC->PCONP |= (1UL << 9U);
    NVIC_DisableIRQ(RTC_IRQn);
    LPC_RTC->CCR = RTC_DIVIDER_RESET;
    LPC_RTC->CIIR = 0U;
    LPC_RTC->AMR = RTC_ALARM_DISABLED;
    LPC_RTC->ILR = RTC_ALL_FLAGS;
    alarm_triggered = false;
    alarm_programmed = false;

    if (!RTC_TimeDateFromSeconds(seconds, &dateTime)) {
        (void)RTC_TimeDateFromSeconds((time_t)0, &dateTime);
    }
    RTC_SetTimeDate(&dateTime);

    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_GetTimeDate(struct tm *dateTime)
{
    uint32_t low;
    uint32_t middle;
    uint32_t high;

    if (dateTime == NULL) {
        return;
    }

    /* Repetir se o segundo mudou durante a leitura dos tres registos. */
    do {
        low = LPC_RTC->CTIME0;
        middle = LPC_RTC->CTIME1;
        high = LPC_RTC->CTIME2;
    } while (low != LPC_RTC->CTIME0);

    dateTime->tm_sec = (int)(low & 0x3FUL);
    dateTime->tm_min = (int)((low >> 8U) & 0x3FUL);
    dateTime->tm_hour = (int)((low >> 16U) & 0x1FUL);
    dateTime->tm_wday = (int)((low >> 24U) & 0x07UL);
    dateTime->tm_mday = (int)(middle & 0x1FUL);
    dateTime->tm_mon = (int)((middle >> 8U) & 0x0FUL) - 1;
    dateTime->tm_year = (int)((middle >> 16U) & 0x0FFFUL) - 1900;
    dateTime->tm_yday = (int)(high & 0x01FFUL) - 1;
    dateTime->tm_isdst = 0;
}

void RTC_SetTimeDate(const struct tm *dateTime)
{
    uint64_t days;
    uint16_t dayOfYear;

    if (!RTC_DaysFromDate(dateTime, &days, &dayOfYear)) {
        return;
    }

    LPC_RTC->CCR = 0U;
    LPC_RTC->YEAR = (uint16_t)((int64_t)dateTime->tm_year + 1900LL);
    LPC_RTC->MONTH = (uint8_t)(dateTime->tm_mon + 1);
    LPC_RTC->DOM = (uint8_t)dateTime->tm_mday;
    LPC_RTC->DOW = (uint8_t)((days + 4ULL) % 7ULL);
    LPC_RTC->DOY = dayOfYear;
    LPC_RTC->HOUR = (uint8_t)dateTime->tm_hour;
    LPC_RTC->MIN = (uint8_t)dateTime->tm_min;
    LPC_RTC->SEC = (uint8_t)dateTime->tm_sec;
    LPC_RTC->CCR = RTC_CLOCK_ENABLE;
}

void RTC_SetSeconds(time_t seconds)
{
    struct tm dateTime;

    if (RTC_TimeDateFromSeconds(seconds, &dateTime)) {
        RTC_SetTimeDate(&dateTime);
    }
}

time_t RTC_GetSeconds(void)
{
    struct tm dateTime;
    uint64_t days;
    uint16_t dayOfYear;
    uint64_t seconds;
    time_t converted;

    RTC_GetTimeDate(&dateTime);
    if (!RTC_DaysFromDate(&dateTime, &days, &dayOfYear)) {
        return (time_t)0;
    }

    seconds = days * SECONDS_PER_DAY +
              (uint64_t)dateTime.tm_hour * 3600ULL +
              (uint64_t)dateTime.tm_min * 60ULL +
              (uint64_t)dateTime.tm_sec;
    converted = (time_t)seconds;
    if ((uint64_t)converted != seconds) {
        return (time_t)0;
    }
    return converted;
}

void RTC_SetAlarm(const struct tm *dateTime)
{
    uint64_t days;
    uint16_t dayOfYear;

    if (!RTC_DaysFromDate(dateTime, &days, &dayOfYear)) {
        return;
    }

    NVIC_DisableIRQ(RTC_IRQn);
    LPC_RTC->AMR = RTC_ALARM_DISABLED;
    LPC_RTC->ILR = RTC_ALARM_FLAG;
    LPC_RTC->ALMIN = (uint8_t)dateTime->tm_min;
    LPC_RTC->ALHOUR = (uint8_t)dateTime->tm_hour;
    LPC_RTC->ALDOM = (uint8_t)dateTime->tm_mday;
    LPC_RTC->ALMON = (uint8_t)(dateTime->tm_mon + 1);
    LPC_RTC->ALYEAR = (uint16_t)((int64_t)dateTime->tm_year + 1900LL);
    alarm_triggered = false;
    alarm_programmed = true;
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_EnableAlarm(void)
{
    if (!alarm_programmed) {
        return;
    }

    NVIC_DisableIRQ(RTC_IRQn);
    LPC_RTC->ILR = RTC_ALARM_FLAG;
    NVIC_ClearPendingIRQ(RTC_IRQn);
    alarm_triggered = false;
    LPC_RTC->AMR = RTC_ALARM_FIELDS;
    NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_DisableAlarm(void)
{
    NVIC_DisableIRQ(RTC_IRQn);
    LPC_RTC->AMR = RTC_ALARM_DISABLED;
    LPC_RTC->ILR = RTC_ALARM_FLAG;
    NVIC_ClearPendingIRQ(RTC_IRQn);
    alarm_triggered = false;
    NVIC_EnableIRQ(RTC_IRQn);
}

bool RTC_AlarmTriggered(void)
{
    uint32_t previousMask = __get_PRIMASK();
    bool occurred;

    __disable_irq();
    occurred = alarm_triggered;
    alarm_triggered = false;
    if (previousMask == 0U) {
        __enable_irq();
    }
    return occurred;
}

void RTC_IRQHandler(void)
{
    uint8_t pending = LPC_RTC->ILR & RTC_ALL_FLAGS;

    if ((pending & RTC_ALARM_FLAG) != 0U) {
        /* Uma ocorrencia por programacao, mesmo sem comparar segundos. */
        LPC_RTC->AMR = RTC_ALARM_DISABLED;
        alarm_triggered = true;
    }
    LPC_RTC->ILR = pending;
}
