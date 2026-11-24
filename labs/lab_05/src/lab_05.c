#include "delay.h"
#include "lcdtext.h"
#include <stdint.h>

int main(void)
{
    uint8_t cycle;

    if (DELAY_Init() != 0) {
        while (1) {
        }
    }

    LCD_init();

    while (1) {
        /* Cinco pares de 1 s: alternancia durante 10 s. */
        for (cycle = 0U; cycle < 5U; cycle++) {
            LCD_Clear();
            LCD_SetCursor(0U, 0U);
            LCD_WriteString("TESTE LCD");
            DELAY_Milliseconds(1000U);

            LCD_Clear();
            LCD_SetCursor(1U, 0U);
            LCD_WriteString("TESTE LCD");
            DELAY_Milliseconds(1000U);
        }

        LCD_Clear();
        DELAY_Milliseconds(500U);
    }
}
