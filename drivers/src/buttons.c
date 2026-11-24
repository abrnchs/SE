#include "buttons.h"
#include "LPC17xx.h"
#include <stdint.h>

/* A, B e C usam, respetivamente, P2.0, P2.1 e P2.2.
 * O modulo fecha cada entrada ao GND quando o botao e premido.
 */
#define BUTTONS_GPIO_MASK  0x07UL
#define BUTTONS_PIN_MASK   0x3FUL

void BUTTONS_Init(void)
{
    /* Os dois bits de funcao de cada pino ficam em 00 (GPIO). */
    LPC_PINCON->PINSEL4 &= ~BUTTONS_PIN_MASK;

    /* 00 em PINMODE4 ativa o pull-up interno de cada entrada. */
    LPC_PINCON->PINMODE4 &= ~BUTTONS_PIN_MASK;
    LPC_GPIO2->FIODIR &= ~BUTTONS_GPIO_MASK;
}

BUTTONS_TypeDef BUTTONS_Read(void)
{
    uint32_t levels = LPC_GPIO2->FIOPIN;

    /* Premido = nivel baixo; cada bit do resultado representa um botao. */
    return (BUTTONS_TypeDef)((~levels) & BUTTONS_GPIO_MASK);
}

BUTTONS_TypeDef BUTTONS_Wait(void)
{
    BUTTONS_TypeDef pressed;

    do {
        pressed = BUTTONS_Read();
    } while (pressed == BUTTONS_NONE);

    return pressed;
}
