#include "spi.h"
#include "stm32l4xx_hal.h"

#define RST_GPIO GPIOB
#define RST_OFFSET 1

typedef enum {
    MFRC_CommandReg   	= 0x01,
    MFRC_ComIEnReg   	= 0x02,
    MFRC_DivIEnReg      = 0x03,
    MFRC_ErrorReg       = 0x06,
    MFRC_Status1Reg     = 0x07,
    MFRC_FIFODATAReg    = 0x09,
    MFRC_VersionReg     = 0x37,
	MFRC_TxModeReg		= 0x12,
	MFRC_RxModeReg		= 0x13,
	MFRC_ModWidthReg	= 0x24
} MFRC_Register;

typedef enum {
    MFRC_CMD_IDLE         = 0x00,
    MFRC_CMD_MEM          = 0x01,
    MFRC_CMD_GENERATE_RANDOM_ID = 0x02,
    MFRC_CMD_CALC_CRC     = 0x03,
    MFRC_CMD_TRANSMIT     = 0x04,
    MFRC_CMD_NO_CMD_CHANGE= 0x07,
    MFRC_CMD_RECEIVE      = 0x08,
    MFRC_CMD_TRANSCEIVE   = 0x0C,
    MFRC_CMD_AUTHENT      = 0x0E,
    MFRC_CMD_SOFT_RESET   = 0x0F
} MFRC_Command;

uint8_t RFIDReadRegister(MFRC_Register);
void RFIDWriteRegister(MFRC_Register, uint8_t);
void MFRC522Init(void);


