#include "spi.h"
#include "stm32l4xx_hal.h"

#define RST_GPIO GPIOB
#define RST_OFFSET 1

typedef enum {
    MFRC_CommandReg       = 0x01,  // Starts and stops command execution
    MFRC_ComIEnReg        = 0x02,  // Enables/disables interrupt request bits
    MFRC_DivIEnReg        = 0x03,  // Enables/disables interrupt request bits
    MFRC_ComIrqReg        = 0x04,  // Interrupt request bits
    MFRC_DivIrqReg        = 0x05,  // Interrupt request bits
    MFRC_ErrorReg         = 0x06,  // Error flags
    MFRC_Status1Reg       = 0x07,  // Communication status
    MFRC_Status2Reg       = 0x08,  // Receiver and transmitter status
    MFRC_FIFODATAReg      = 0x09,  // FIFO data register
    MFRC_FIFOLevelReg     = 0x0A,  // Number of bytes stored in FIFO
    MFRC_ControlReg       = 0x0C,  // Misc control bits
    MFRC_BitFramingReg    = 0x0D,  // Bit-level framing
    MFRC_CollReg          = 0x0E,  // Collision detection

    MFRC_ModeReg          = 0x11,  // Defines internal modes
    MFRC_TxModeReg        = 0x12,  // Transmitter data rate settings
    MFRC_RxModeReg        = 0x13,  // Receiver data rate settings
    MFRC_TxControlReg     = 0x14,  // Controls antenna drivers TX1 and TX2
    MFRC_TxASKReg         = 0x15,  // Controls ASK modulation
    MFRC_ModWidthReg      = 0x24,  // Modulation width setting

    MFRC_RFCfgReg         = 0x26,  // Receiver gain settings
    MFRC_GsNReg           = 0x27,  // Defines conductance in reception
    MFRC_CWGsPReg         = 0x28,  // Defines conductance of the driver pins
    MFRC_ModGsPReg        = 0x29,  // Modulation conductance

    MFRC_TModeReg         = 0x2A,  // Timer mode
    MFRC_TPrescalerReg    = 0x2B,  // Timer prescaler
    MFRC_TReloadRegH      = 0x2C,  // Timer reload (high)
    MFRC_TReloadRegL      = 0x2D,  // Timer reload (low)

    MFRC_VersionReg       = 0x37,  // Chip version (should return 0x92 or 0x91)
    MFRC_CRCResultRegH    = 0x21,  // CRC calculation result (high)
    MFRC_CRCResultRegL    = 0x22   // CRC calculation result (low)
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


