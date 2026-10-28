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

#include "delay.h"
#include "led.h"

int main(void)
{

    uint32_t start_millis;
    uint32_t measured_millis;


    LED_Init(RED, false);
    LED_Init(GREEN, false);
    LED_Init(BLUE, false);


    /* O gestor precisa de iniciar o SysTick e o Timer2 */

    if (DELAY_Init() < 0) {
        LED_On(RED);

        while (1) {
        }
    }

    while (1) {

        /* Medir com o SysTick uma espera de 1 000 000 us do Timer2. */

        start_millis = DELAY_GetMillis();

        DELAY_Microseconds(1000000U);

        measured_millis = DELAY_GetElapsedMillis(start_millis);

        if (measured_millis >= 995U && measured_millis <= 1005U) {
            LED_On(GREEN);
        } else {
            LED_On(RED);
        }

        DELAY_Milliseconds(2000U);
        LED_Off(RED);
        LED_Off(GREEN);
        LED_Off(BLUE);
    }
}
