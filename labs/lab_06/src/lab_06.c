/**
 * @file lab_06.c
 * @brief Teste dos botoes, do relogio RTC e do alarme no LCD 16x2.
 */
#include "buttons.h"
#include "delay.h"
#include "lcdtext.h"
#include "led.h"
#include "rtc.h"
#include <stdint.h>

/* 17/11/2026 12:00:00 UTC, em segundos desde 01/01/1970. */
#define INITIAL_TIME_SECONDS 1794916800L
#define ALARM_LED_TIME_MS    2000U

typedef enum {
    ALARM_PENDING,
    ALARM_CANCELED,
    ALARM_FIRED
} AlarmState;

static void WriteTwoDigits(char *destination, uint8_t value)
{
    destination[0] = (char)('0' + value / 10U);
    destination[1] = (char)('0' + value % 10U);
}

static void ShowDateTime(const struct tm *dateTime)
{
    char line[17] = "00/00/0000 00:00";
    uint16_t year = (uint16_t)(dateTime->tm_year + 1900);

    WriteTwoDigits(&line[0], (uint8_t)dateTime->tm_mday);
    WriteTwoDigits(&line[3], (uint8_t)(dateTime->tm_mon + 1));
    WriteTwoDigits(&line[6], (uint8_t)(year / 100U));
    WriteTwoDigits(&line[8], (uint8_t)(year % 100U));
    WriteTwoDigits(&line[11], (uint8_t)dateTime->tm_hour);
    WriteTwoDigits(&line[14], (uint8_t)dateTime->tm_min);

    LCD_SetCursor(0U, 0U);
    LCD_WriteString(line);
}

static void ShowButtonsAndAlarm(uint8_t seconds,
                                BUTTONS_TypeDef buttons,
                                AlarmState state)
{
    char line[17] = "00 B:0 PENDENTE ";
    const char *status;
    uint8_t index;

    if (state == ALARM_CANCELED) {
        status = "CANCELADO";
    } else if (state == ALARM_FIRED) {
        status = "DISPAROU ";
    } else {
        status = "PENDENTE ";
    }

    WriteTwoDigits(&line[0], seconds);
    line[5] = (char)('0' + (uint8_t)buttons);
    for (index = 0U; index < 9U; index++) {
        line[7U + index] = status[index];
    }

    LCD_SetCursor(1U, 0U);
    LCD_WriteString(line);
}

int main(void)
{
    struct tm alarmTime;
    struct tm now;
    BUTTONS_TypeDef buttons;
    uint32_t ledStart = 0U;
    uint8_t lastSecond = 60U;
    uint8_t lastButtons = 0xFFU;
    AlarmState state = ALARM_PENDING;
    AlarmState lastState = (AlarmState)0xFFU;
    bool ledOn = false;
    bool secondChanged;

    if (DELAY_Init() != 0) {
        while (1) {
        }
    }

    LCD_init();
    LED_Init(GREEN, false);
    BUTTONS_Init();

    /* Primeiro teste: BUTTONS_Wait so regressa quando um botao e premido. */
    LCD_Clear();
    LCD_WriteString("Prima A, B ou C");
    buttons = BUTTONS_Wait();
    LCD_SetCursor(1U, 0U);
    LCD_WriteString("Wait = ");
    LCD_WriteChar((char)('0' + (uint8_t)buttons));
    DELAY_Milliseconds(750U);

    /* Esperar que o botao seja solto antes de iniciar o teste do alarme. */
    while (BUTTONS_Read() != BUTTONS_NONE) {
        DELAY_Milliseconds(20U);
    }
    DELAY_Milliseconds(20U);

    RTC_Init((time_t)INITIAL_TIME_SECONDS);
    RTC_GetTimeDate(&alarmTime);
    alarmTime.tm_min += 3;
    RTC_SetAlarm(&alarmTime);
    RTC_EnableAlarm();
    LCD_Clear();

    while (1) {
        /* BUTTONS_Read e imediato: 0 = nenhum; 1, 2 e 4 = A, B e C. */
        buttons = BUTTONS_Read();

        if (RTC_AlarmTriggered() && state == ALARM_PENDING) {
            state = ALARM_FIRED;
            LED_On(GREEN);
            ledOn = true;
            ledStart = DELAY_GetMillis();
        }

        if (state == ALARM_PENDING &&
            (((uint8_t)buttons & (uint8_t)BUTTONS_C) != 0U)) {
            RTC_DisableAlarm();
            state = ALARM_CANCELED;
        }

        if (ledOn && DELAY_GetElapsedMillis(ledStart) >= ALARM_LED_TIME_MS) {
            LED_Off(GREEN);
            ledOn = false;
        }

        RTC_GetTimeDate(&now);
        secondChanged = ((uint8_t)now.tm_sec != lastSecond);
        if (secondChanged) {
            ShowDateTime(&now);
            lastSecond = (uint8_t)now.tm_sec;
        }
        if ((uint8_t)buttons != lastButtons || state != lastState ||
            secondChanged) {
            ShowButtonsAndAlarm((uint8_t)now.tm_sec, buttons, state);
            lastButtons = (uint8_t)buttons;
            lastState = state;
        }

        DELAY_Milliseconds(20U);
    }
}
