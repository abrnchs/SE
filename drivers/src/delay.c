#include "delay.h"
#include "LPC17xx.h"

/* Atualizada pela interrupcao; privada deste ficheiro. */
static volatile uint32_t elapsed_millis = 0U;

int DELAY_Init(void)
{
    uint32_t ticks_per_millisecond;
    uint32_t timer2_clock_hz;
    uint32_t ticks_per_microsecond;
    uint32_t pclk_selection;

    SystemCoreClockUpdate();
    ticks_per_millisecond = SystemCoreClock / 1000U;
    timer2_clock_hz = SystemCoreClock / 4U;

    if (ticks_per_millisecond == 0U ||
        timer2_clock_hz < 1000000U ||
        (timer2_clock_hz % 1000000U) != 0U) {
        return -1;
    }
    ticks_per_microsecond = timer2_clock_hz / 1000000U;

    elapsed_millis = 0U;

    if (SysTick_Config(ticks_per_millisecond) != 0U) {
        return -1;
    }

    /* Ligar o Timer2: bit 22 de PCONP. */
    LPC_SC->PCONP = LPC_SC->PCONP | (1UL << 22U);

    /* PCLKSEL1[13:12] = 00: relogio do Timer2 = SystemCoreClock / 4. */
    pclk_selection = LPC_SC->PCLKSEL1;
    pclk_selection = pclk_selection & ~(1UL << 12U);
    pclk_selection = pclk_selection & ~(1UL << 13U);
    LPC_SC->PCLKSEL1 = pclk_selection;

    LPC_TIM2->TCR = 0U;                         /* Parar antes de configurar. */
    LPC_TIM2->CTCR = 0U;                        /* Contar impulsos do relogio. */
    LPC_TIM2->MCR = 0U;                         /* Sem interrupcoes ou match. */
    LPC_TIM2->PR = ticks_per_microsecond - 1U; /* TC avanca a cada 1 us. */
    LPC_TIM2->TCR = 2U;                         /* Reiniciar TC e PC. */
    LPC_TIM2->TCR = 1U;                         /* Iniciar contador livre. */

    return 0;
}

void DELAY_Milliseconds(uint32_t millis)
{
    uint32_t start = DELAY_GetMillis();

    while (DELAY_GetElapsedMillis(start) < millis) {
        /* A espera depende apenas dos ticks de 1 ms do SysTick. */
    }
}

void DELAY_Microseconds(uint32_t waitUs)
{
    uint32_t start = LPC_TIM2->TC;
    uint32_t elapsed = 0U;

    while (elapsed < waitUs) {
        /* TC avanca uma unidade por microssegundo e nunca e reiniciado aqui. */
        elapsed = (uint32_t)(LPC_TIM2->TC - start);
    }
}

uint32_t DELAY_GetMillis(void)
{
    return elapsed_millis;
}

uint32_t DELAY_GetElapsedMillis(uint32_t start)
{
    uint32_t now = DELAY_GetMillis();

    return (uint32_t)(now - start);
}

void SysTick_Handler(void)
{
    elapsed_millis++;
}
