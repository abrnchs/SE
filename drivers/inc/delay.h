/**
 * @file delay.h
 * @brief Interface publica do gestor de tempo com SysTick e Timer2.
 */
#ifndef DRIVERS_DELAY_H
#define DRIVERS_DELAY_H

#include <stdint.h>

/**
 * @brief Inicializa o SysTick a 1 ms e o Timer2 a 1 us.
 * @return 0 em caso de sucesso; valor negativo se a configuracao falhar.
 */
int DELAY_Init(void);

/**
 * @brief Aguarda o numero de milissegundos indicado.
 * @param millis Duracao da espera em milissegundos.
 * @note Chamar DELAY_Init() com sucesso antes desta funcao.
 */
void DELAY_Milliseconds(uint32_t millis);

/**
 * @brief Aguarda o numero de microssegundos indicado.
 * @param waitUs Duracao da espera em microssegundos.
 * @note Usa polling do Timer2; chamar DELAY_Init() com sucesso antes.
 */
void DELAY_Microseconds(uint32_t waitUs);

/**
 * @brief Consulta a base de tempo do gestor.
 * @return Numero de milissegundos desde a inicializacao.
 */
uint32_t DELAY_GetMillis(void);

/**
 * @brief Calcula o tempo decorrido desde um instante anterior.
 * @param start Valor obtido anteriormente com DELAY_GetMillis().
 * @return Tempo decorrido em milissegundos, mesmo apos o contador voltar a zero.
 */
uint32_t DELAY_GetElapsedMillis(uint32_t start);

#endif /* DRIVERS_DELAY_H */
