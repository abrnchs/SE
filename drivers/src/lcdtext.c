#include "lcdtext.h"
#include "delay.h"
#include "LPC17xx.h"
#include <stdbool.h>
#include <stddef.h>

/* Ligacoes da proposta G08 Lab04: DB4..DB7=P0.0..P0.3, RS=P0.4, E=P0.5.
 * R/W do LCD deve estar ligado a GND; DB0..DB3 ficam desligados.
 */
#define LCD_DATA_MASK  0x0FUL
#define LCD_RS_MASK    (1UL << 4U)
#define LCD_E_MASK     (1UL << 5U)
#define LCD_GPIO_MASK  (LCD_DATA_MASK | LCD_RS_MASK | LCD_E_MASK)

static void LCD_Write4bits(uint8_t nibble)
{
    LPC_GPIO0->FIOCLR = LCD_DATA_MASK;
    LPC_GPIO0->FIOSET = (uint32_t)(nibble & 0x0FU);
}

static void LCD_PulseEnable(void)
{
    DELAY_Microseconds(1U);
    LPC_GPIO0->FIOSET = LCD_E_MASK;
    DELAY_Microseconds(1U);
    LPC_GPIO0->FIOCLR = LCD_E_MASK;
    DELAY_Microseconds(1U);
}

static void LCD_WriteByte(uint8_t value, bool isData)
{
    if (isData) {
        LPC_GPIO0->FIOSET = LCD_RS_MASK;
    } else {
        LPC_GPIO0->FIOCLR = LCD_RS_MASK;
    }

    LCD_Write4bits((uint8_t)(value >> 4U));
    LCD_PulseEnable();
    LCD_Write4bits((uint8_t)(value & 0x0FU));
    LCD_PulseEnable();
    DELAY_Microseconds(50U);
}

static void LCD_Command(uint8_t command)
{
    LCD_WriteByte(command, false);

    if (command == 0x01U || command == 0x02U) {
        DELAY_Microseconds(2000U);
    }
}

void LCD_init(void)
{
    /* Cada P0.0..P0.5 usa dois bits de PINSEL0; 00 seleciona GPIO. */
    LPC_PINCON->PINSEL0 &= ~0x00000FFFUL;
    LPC_GPIO0->FIOCLR = LCD_GPIO_MASK;
    LPC_GPIO0->FIODIR |= LCD_GPIO_MASK;

    DELAY_Milliseconds(50U);

    /* Sequencia especial de arranque antes de existir transferencia 4-bit. */
    LCD_Write4bits(0x03U);
    LCD_PulseEnable();
    DELAY_Microseconds(5000U);

    LCD_Write4bits(0x03U);
    LCD_PulseEnable();
    DELAY_Microseconds(150U);

    LCD_Write4bits(0x03U);
    LCD_PulseEnable();
    DELAY_Microseconds(150U);

    LCD_Write4bits(0x02U);
    LCD_PulseEnable();
    DELAY_Microseconds(150U);

    LCD_Command(0x28U); /* 4 bits, 2 linhas, matriz 5x8. */
    LCD_Command(0x08U); /* Desligar apresentacao durante a configuracao. */
    LCD_Clear();
    LCD_Command(0x06U); /* Avancar cursor apos cada carater. */
    LCD_Command(0x0CU); /* Ligar apresentacao; cursor oculto. */
}

void LCD_WriteChar(char c)
{
    LCD_WriteByte((uint8_t)c, true);
}

void LCD_WriteString(const char *str)
{
    if (str == NULL) {
        return;
    }

    while (*str != '\0') {
        LCD_WriteChar(*str);
        str++;
    }
}

void LCD_Clear(void)
{
    LCD_Command(0x01U);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    if (row > 1U || col > 15U) {
        return;
    }

    address = (row == 0U) ? col : (uint8_t)(0x40U + col);
    LCD_Command((uint8_t)(0x80U | address));
}
