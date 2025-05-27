#include "spi.h"

typedef enum {
    MFRC_CommandReg      = 0x01,
    MFRC_ComIEnReg       = 0x02,
    MFRC_DivIEnReg       = 0x03,
    MFRC_ErrorReg        = 0x06,
    MFRC_Status1Reg      = 0x07,
    MFRC_FIFODATAReg     = 0x09,
    MFRC_VersionReg      = 0x37,
} MFRC_Register;

uint8_t RFIDReadRegister(MFRC_Register);
void RFIDWriteRegister(MMFRC_Register, uint8_t);


