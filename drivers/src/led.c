#include "led.h"
#include "LPC17xx.h"

/* Ligações do LED1 no esquema elétrico da LPCXpresso LPC1769. */
#define LED_RED_GPIO    LPC_GPIO0
#define LED_RED_BIT     22U

#define LED_GREEN_GPIO  LPC_GPIO3
#define LED_GREEN_BIT   25U

#define LED_BLUE_GPIO   LPC_GPIO3
#define LED_BLUE_BIT    26U

void LED_Init(LEDTypedef_t led, bool state)
{
    LPC_GPIO_TypeDef *gpio;
    uint32_t bit_mask;
    uint32_t pinsel;

    switch (led) {
    case RED:
        gpio = LED_RED_GPIO;
        bit_mask = 1UL << LED_RED_BIT;

        /* P0.22: os bits 13 e 12 de PINSEL1 devem ficar a zero. */
        pinsel = LPC_PINCON->PINSEL1;
        pinsel = pinsel & ~(1UL << 12U);
        pinsel = pinsel & ~(1UL << 13U);
        LPC_PINCON->PINSEL1 = pinsel;
        break;

    case GREEN:
        gpio = LED_GREEN_GPIO;
        bit_mask = 1UL << LED_GREEN_BIT;

        /* P3.25: os bits 19 e 18 de PINSEL7 devem ficar a zero. */
        pinsel = LPC_PINCON->PINSEL7;
        pinsel = pinsel & ~(1UL << 18U);
        pinsel = pinsel & ~(1UL << 19U);
        LPC_PINCON->PINSEL7 = pinsel;
        break;

    case BLUE:
        gpio = LED_BLUE_GPIO;
        bit_mask = 1UL << LED_BLUE_BIT;

        /* P3.26: os bits 21 e 20 de PINSEL7 devem ficar a zero. */
        pinsel = LPC_PINCON->PINSEL7;
        pinsel = pinsel & ~(1UL << 20U);
        pinsel = pinsel & ~(1UL << 21U);
        LPC_PINCON->PINSEL7 = pinsel;
        break;

    default:
        return;
    }

    /* As três cores do LED1 são ativas a zero. */
    if (state) {
        gpio->FIOCLR = bit_mask;
    } else {
        gpio->FIOSET = bit_mask;
    }

    /* Preparar o nível antes de configurar o pino como saída. */
    gpio->FIODIR = gpio->FIODIR | bit_mask;
}

void LED_On(LEDTypedef_t led)
{
    /* Cada cor é ativa a zero: limpar o respetivo bit acende-a. */
    switch (led) {
    case RED:
        LED_RED_GPIO->FIOCLR = 1UL << LED_RED_BIT;
        break;

    case GREEN:
        LED_GREEN_GPIO->FIOCLR = 1UL << LED_GREEN_BIT;
        break;

    case BLUE:
        LED_BLUE_GPIO->FIOCLR = 1UL << LED_BLUE_BIT;
        break;

    default:
        return;
    }
}

void LED_Off(LEDTypedef_t led)
{
    /* Colocar o respetivo bit a um apaga a cor indicada. */
    switch (led) {
    case RED:
        LED_RED_GPIO->FIOSET = 1UL << LED_RED_BIT;
        break;

    case GREEN:
        LED_GREEN_GPIO->FIOSET = 1UL << LED_GREEN_BIT;
        break;

    case BLUE:
        LED_BLUE_GPIO->FIOSET = 1UL << LED_BLUE_BIT;
        break;

    default:
        return;
    }
}

bool LED_GetState(LEDTypedef_t led)
{
    LPC_GPIO_TypeDef *gpio;
    uint32_t bit_mask;
    uint32_t pin_level;

    switch (led) {
    case RED:
        gpio = LED_RED_GPIO;
        bit_mask = 1UL << LED_RED_BIT;
        break;

    case GREEN:
        gpio = LED_GREEN_GPIO;
        bit_mask = 1UL << LED_GREEN_BIT;
        break;

    case BLUE:
        gpio = LED_BLUE_GPIO;
        bit_mask = 1UL << LED_BLUE_BIT;
        break;

    default:
        return false;
    }

    /* Ler apenas o bit do pino escolhido. */
    pin_level = gpio->FIOPIN & bit_mask;

    /* O LED está aceso quando o pino está a zero. */
    if (pin_level == 0U) {
        return true;
    }

    return false;
}

void LED_Toggle(LEDTypedef_t led)
{
    if (LED_GetState(led)) {
        LED_Off(led);
    } else {
        LED_On(led);
    }
}
