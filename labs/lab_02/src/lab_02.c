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
    uint32_t red_phase_start;

    /* Comecar com as tres componentes apagadas. */
    LED_Init(RED, false);
    LED_Init(GREEN, false);
    LED_Init(BLUE, false);

    /* Sem SysTick nao e possivel medir os intervalos pedidos. */
    if (DELAY_Init() < 0) {
        LED_On(RED);  /* Vermelho fixo indica erro de inicializacao. */
        while (1) {
        }
    }

    while (1) {
        /* Guardar o instante em que comeca a fase vermelha. */
        red_phase_start = DELAY_GetMillis();

        /* Alternar o vermelho a cada 500 ms durante 5 segundos. */
        while (DELAY_GetElapsedMillis(red_phase_start) < 5000U) {
            LED_Toggle(RED);
            DELAY_Milliseconds(500U);
        }

        /* Terminar a fase vermelha e acender o verde por 2 segundos. */
        LED_Off(RED);
        LED_On(GREEN);
        DELAY_Milliseconds(2000U);
        LED_Off(GREEN);
    }
}
