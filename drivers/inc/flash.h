/**
 * @file flash.h
 * @brief Operacoes IAP sobre a memoria flash interna do LPC1769.
 *
 * Os valores devolvidos sao os codigos de estado IAP do manual UM10360.
 * Uma aplicacao deve reservar no seu mapa de memoria os setores usados para
 * dados. A aplicacao deve ainda reservar os 32 bytes finais da RAM local,
 * para que a pilha nao os ocupe durante a escrita ou o apagamento IAP.
 */
#ifndef DRIVERS_FLASH_H
#define DRIVERS_FLASH_H

/** Codigos de estado IAP definidos pelo LPC1769. */
typedef enum {
    FLASH_STATUS_SUCCESS = 0U,
    FLASH_STATUS_INVALID_COMMAND = 1U,
    FLASH_STATUS_SRC_ADDR_ERROR = 2U,
    FLASH_STATUS_DST_ADDR_ERROR = 3U,
    FLASH_STATUS_SRC_ADDR_NOT_MAPPED = 4U,
    FLASH_STATUS_DST_ADDR_NOT_MAPPED = 5U,
    FLASH_STATUS_COUNT_ERROR = 6U,
    FLASH_STATUS_INVALID_SECTOR = 7U,
    FLASH_STATUS_SECTOR_NOT_BLANK = 8U,
    FLASH_STATUS_SECTOR_NOT_PREPARED = 9U,
    FLASH_STATUS_COMPARE_ERROR = 10U,
    FLASH_STATUS_BUSY = 11U
} FLASH_StatusCode;

/**
 * @brief Apaga um setor da flash interna.
 * @param sector Numero do setor, de 0 a 29.
 * @return Codigo IAP: FLASH_STATUS_SUCCESS se o setor foi apagado; outro
 *         codigo IAP em caso de erro.
 * @warning Apagar um setor que contenha o programa impede a sua execucao.
 */
unsigned int FLASH_EraseSector(unsigned int sector);

/**
 * @brief Copia um bloco da RAM para a flash interna.
 * @param dstAddr Endereco de destino na flash, alinhado a 256 bytes.
 * @param srcAddr Endereco de origem na RAM, alinhado a 4 bytes.
 * @param size Dimensao do bloco: 256, 512, 1024 ou 4096 bytes.
 * @return Codigo IAP: FLASH_STATUS_SUCCESS se a escrita terminou; outro
 *         codigo IAP em caso de erro.
 * @details A funcao prepara automaticamente os setores abrangidos. A zona
 *          de destino deve estar previamente apagada. Para gravar uma
 *          estrutura menor que 256 bytes, copie-a para um buffer de 256
 *          bytes na RAM, preenchido com 0xFF, e grave esse buffer.
 */
unsigned int FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size);

/**
 * @brief Compara os dados na flash com outro bloco de memoria.
 * @param dstAddr Endereco na flash, alinhado a 4 bytes.
 * @param srcAddr Endereco dos dados de referencia, alinhado a 4 bytes.
 * @param size Numero positivo de bytes a comparar, multiplo de 4.
 * @return FLASH_STATUS_SUCCESS se os blocos sao iguais;
 *         FLASH_STATUS_COMPARE_ERROR se diferem; outro codigo IAP se a
 *         comparacao nao pode ser feita.
 * @note O comando IAP Compare nao e fiavel para os primeiros 64 bytes da
 *       flash, devido ao remapeamento dos vetores de interrupcao.
 */
unsigned int FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size);

#endif /* DRIVERS_FLASH_H */
