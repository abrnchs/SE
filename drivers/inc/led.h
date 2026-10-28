/**
 * @file led.h
 * @brief Interface publica do gestor das componentes do LED RGB.
 */
#ifndef DRIVERS_LED_H
#define DRIVERS_LED_H

#include <stdbool.h>

/** Identifica uma das três componentes do LED RGB da placa. */
typedef enum {
    RED = 0,
    GREEN,
    BLUE
} LEDTypedef_t;

/**
 * @brief Inicializa o LED indicado com o estado pedido.
 * @param led Componente do LED a inicializar.
 * @param state true para aceso; false para apagado.
 */
void LED_Init(LEDTypedef_t led, bool state);

/**
 * @brief Consulta o estado do LED indicado.
 * @param led Componente do LED a consultar.
 * @return true se estiver aceso; false se estiver apagado.
 */
bool LED_GetState(LEDTypedef_t led);

/**
 * @brief Acende o LED indicado.
 * @param led Componente do LED a acender.
 */
void LED_On(LEDTypedef_t led);

/**
 * @brief Apaga o LED indicado.
 * @param led Componente do LED a apagar.
 */
void LED_Off(LEDTypedef_t led);

/**
 * @brief Troca o estado do LED indicado.
 * @param led Componente do LED a alternar.
 */
void LED_Toggle(LEDTypedef_t led);

#endif /* DRIVERS_LED_H */
