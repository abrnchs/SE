#include "flash.h"
#include "LPC17xx.h"
#include <stdint.h>

/* UM10360, capitulo 32: IAP na ROM (bit 0 indica modo Thumb). */
#define FLASH_IAP_ENTRY          0x1FFF1FF1UL
#define FLASH_IAP_PREPARE        50U
#define FLASH_IAP_COPY           51U
#define FLASH_IAP_ERASE          52U
#define FLASH_IAP_COMPARE        56U

#define FLASH_SIZE_BYTES         0x80000UL
#define FLASH_SMALL_AREA_END     0x10000UL
#define FLASH_SMALL_SECTOR_BYTES 0x1000UL
#define FLASH_LARGE_SECTOR_BYTES 0x8000UL
#define FLASH_FIRST_LARGE_SECTOR 16U
#define FLASH_LAST_SECTOR        29U

typedef void (*FLASH_IAP_Entry)(uint32_t command[], uint32_t result[]);

static unsigned int FLASH_CallIAP(uint32_t command[5])
{
    uint32_t result[5] = {0U};
    uint32_t previousMask = __get_PRIMASK();
    FLASH_IAP_Entry entry = (FLASH_IAP_Entry)FLASH_IAP_ENTRY;

    /* Os vetores e os handlers estao na flash, indisponivel ao apagar/gravar. */
    __disable_irq();
    entry(command, result);
    if (previousMask == 0U) {
        __enable_irq();
    }

    return result[0];
}

static unsigned int FLASH_SectorForAddress(uint32_t address)
{
    if (address < FLASH_SMALL_AREA_END) {
        return address / FLASH_SMALL_SECTOR_BYTES;
    }
    return FLASH_FIRST_LARGE_SECTOR +
           (address - FLASH_SMALL_AREA_END) / FLASH_LARGE_SECTOR_BYTES;
}

static unsigned int FLASH_PrepareSectors(unsigned int first,
                                         unsigned int last)
{
    uint32_t command[5] = {FLASH_IAP_PREPARE, first, last, 0U, 0U};
    return FLASH_CallIAP(command);
}

unsigned int FLASH_EraseSector(unsigned int sector)
{
    uint32_t command[5];
    unsigned int status;

    if (sector > FLASH_LAST_SECTOR) {
        return FLASH_STATUS_INVALID_SECTOR;
    }

    SystemCoreClockUpdate();
    status = FLASH_PrepareSectors(sector, sector);
    if (status != FLASH_STATUS_SUCCESS) {
        return status;
    }

    command[0] = FLASH_IAP_ERASE;
    command[1] = sector;
    command[2] = sector;
    command[3] = SystemCoreClock / 1000U; /* CCLK em kHz. */
    command[4] = 0U;
    return FLASH_CallIAP(command);
}

unsigned int FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size)
{
    uintptr_t destination = (uintptr_t)dstAddr;
    uintptr_t source = (uintptr_t)srcAddr;
    uint32_t command[5];
    unsigned int firstSector;
    unsigned int lastSector;
    unsigned int status;

    if (size != 256U && size != 512U && size != 1024U && size != 4096U) {
        return FLASH_STATUS_COUNT_ERROR;
    }
    if ((destination & 0xFFU) != 0U) {
        return FLASH_STATUS_DST_ADDR_ERROR;
    }
    if ((source & 0x03U) != 0U) {
        return FLASH_STATUS_SRC_ADDR_ERROR;
    }
    if (destination >= FLASH_SIZE_BYTES ||
        size > FLASH_SIZE_BYTES - destination) {
        return FLASH_STATUS_DST_ADDR_NOT_MAPPED;
    }

    firstSector = FLASH_SectorForAddress((uint32_t)destination);
    lastSector = FLASH_SectorForAddress((uint32_t)(destination + size - 1U));

    SystemCoreClockUpdate();
    status = FLASH_PrepareSectors(firstSector, lastSector);
    if (status != FLASH_STATUS_SUCCESS) {
        return status;
    }

    command[0] = FLASH_IAP_COPY;
    command[1] = (uint32_t)destination;
    command[2] = (uint32_t)source;
    command[3] = size;
    command[4] = SystemCoreClock / 1000U; /* CCLK em kHz. */
    return FLASH_CallIAP(command);
}

unsigned int FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size)
{
    uintptr_t destination = (uintptr_t)dstAddr;
    uintptr_t source = (uintptr_t)srcAddr;
    uint32_t command[5];

    if (size == 0U || (size & 0x03U) != 0U) {
        return FLASH_STATUS_COUNT_ERROR;
    }
    if ((destination & 0x03U) != 0U) {
        return FLASH_STATUS_DST_ADDR_ERROR;
    }
    if ((source & 0x03U) != 0U) {
        return FLASH_STATUS_SRC_ADDR_ERROR;
    }
    if (destination >= FLASH_SIZE_BYTES ||
        size > FLASH_SIZE_BYTES - destination) {
        return FLASH_STATUS_DST_ADDR_NOT_MAPPED;
    }

    command[0] = FLASH_IAP_COMPARE;
    command[1] = (uint32_t)destination;
    command[2] = (uint32_t)source;
    command[3] = size;
    command[4] = 0U;
    return FLASH_CallIAP(command);
}
