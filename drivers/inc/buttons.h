/**
 * @file buttons.h
 * @brief Leitura dos tres botoes do modulo 3-Button Breakout.
 */
#ifndef DRIVERS_BUTTONS_H
#define DRIVERS_BUTTONS_H

/** Cada bit indica um botao atualmente premido. */
typedef enum {
    BUTTONS_NONE = 0U,
    BUTTONS_A = (1U << 0),
    BUTTONS_B = (1U << 1),
    BUTTONS_C = (1U << 2)
} BUTTONS_TypeDef;

/**
 * @brief Configura P2.0, P2.1 e P2.2 como entradas com pull-up.
 * @details Cada botao deve ligar o GPIO correspondente ao GND quando premido.
 */
void BUTTONS_Init(void);

/**
 * @brief Le os tres botoes sem bloquear.
 * @return Bitmap dos botoes premidos (0 a 7), ou BUTTONS_NONE.
 */
BUTTONS_TypeDef BUTTONS_Read(void);

/**
 * @brief Espera por polling ate pelo menos um botao estar premido.
 * @return Bitmap dos botoes premidos no instante da leitura (1 a 7).
 */
BUTTONS_TypeDef BUTTONS_Wait(void);

#endif /* DRIVERS_BUTTONS_H */
