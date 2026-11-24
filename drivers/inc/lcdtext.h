/**
 * @file lcdtext.h
 * @brief Interface publica do gestor de LCD alfanumerico 16x2.
 *
 * O LCD usa uma interface de 4 bits e apenas escrita. Antes de chamar
 * LCD_init(), inicializar o gestor de tempo com DELAY_Init().
 */
#ifndef DRIVERS_LCDTEXT_H
#define DRIVERS_LCDTEXT_H

#include <stdint.h>

/**
 * @brief Inicializa o LCD 16x2 em modo de 4 bits.
 * @pre DELAY_Init() devolveu zero e o LCD esta alimentado.
 */
void LCD_init(void);

/**
 * @brief Escreve um carater na posicao atual do cursor.
 * @param c Carater a apresentar.
 */
void LCD_WriteChar(char c);

/**
 * @brief Escreve uma cadeia de carateres a partir da posicao atual.
 * @param str Cadeia terminada por '\0'; NULL nao produz escrita.
 */
void LCD_WriteString(const char *str);

/**
 * @brief Limpa o mostrador e coloca o cursor no inicio da primeira linha.
 * @details O gestor aguarda a conclusao deste comando antes de regressar.
 */
void LCD_Clear(void);

/**
 * @brief Posiciona o cursor numa das 32 posicoes visiveis.
 * @param row Linha: 0 ou 1.
 * @param col Coluna: 0 a 15.
 * @note Coordenadas fora destes limites sao ignoradas.
 */
void LCD_SetCursor(uint8_t row, uint8_t col);

#endif /* DRIVERS_LCDTEXT_H */
