/*
 * Copyright 2022 NXP
 * NXP confidential.
 * This software is owned or controlled by NXP and may only be used strictly
 * in accordance with the applicable license terms.  By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to
 * be bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

#ifdef __USE_CMSIS

#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#define LED_GREEN_BIT          25U
#define LED_GREEN_MASK         (1UL << LED_GREEN_BIT)
#define LED_PINSEL_SHIFT       18U


#define DELAY_500_MS_COUNT     5000000UL

volatile uint32_t core_clock_hz;


void delay_software(uint32_t count)
{
    volatile uint32_t i;

    for (i = 0U; i < count; ++i) {
        __NOP();
    }
}

void led_green_init(void) {

    LPC_PINCON->PINSEL7 &= ~(3UL << LED_PINSEL_SHIFT);

    LPC_GPIO3->FIOSET = LED_GREEN_MASK;
    LPC_GPIO3->FIODIR |= LED_GREEN_MASK;
}

int main(void)
{
    SystemCoreClockUpdate();

    core_clock_hz = SystemCoreClock;

    led_green_init();

    while (1) {

        LPC_GPIO3->FIOCLR = LED_GREEN_MASK;
        delay_software(DELAY_500_MS_COUNT);

        LPC_GPIO3->FIOSET = LED_GREEN_MASK;
        delay_software(DELAY_500_MS_COUNT);
    }

    return 0;
}
