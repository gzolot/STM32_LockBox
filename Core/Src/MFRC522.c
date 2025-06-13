#include "MFRC522.h"

void MFRC522Init(){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RST_GPIO->MODER &= ~(0b11 << (RST_OFFSET * 2));
	RST_GPIO->MODER |= (0b01 << (RST_OFFSET * 2)); 	// Output mode
	GPIOB->ODR &= ~(1 << RST_OFFSET);           	// Set low for reset
	HAL_Delay(50);
	GPIOB->ODR |= (1 << RST_OFFSET);            	// Set high to enable chip
	HAL_Delay(50);
}

uint8_t RFIDReadRegister(MFRC_Register address){
	uint8_t tx_buffer[2] = {(address << 1) | 0x80, 0x00};
	uint8_t rx_buffer[2];
	spiSendBytes(tx_buffer, 2, rx_buffer);
	return rx_buffer[1];
}


void RFIDWriteRegister(MFRC_Register address, uint8_t tx_byte){
	uint8_t tx_buffer[2] = {(address << 1) & 0x7E, tx_byte};
	uint8_t rx_buffer[2];
	spiSendBytes(tx_buffer, 2, rx_buffer);
}

uint8_t RFIDSendCommand(MFRC_Command command){
	RFIDWriteRegister(MFRC_CommandReg, command);
}

void RFIDInit(){
	RFIDReset();
	//reset baud rates
	RFIDWriteRegister(MFRC_TxModeReg, 0x00);
	RFIDWriteRegister(MFRC_RxModeReg, 0x00);

	//reset ModWidthReg
	RFIDWriteRegister(MFRC_ModWidthReg, 0x26);	// Modulation width determines how long each data bit is transmitted via the ASK signal (1.6 µs typical)

	RFIDWriteRegister(MFRC_TModeReg, 0x80);		// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
	RFIDWriteRegister(MFRC_TPrescalerReg, 0xA9);// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25μs.
	RFIDWriteRegister(MFRC_TReloadRegH, 0x03);	// Reload timer with 0x3E8 = 1000, ie 25ms before timeout (25μs * 1000 = 25ms)
	RFIDWriteRegister(MFRC_TReloadRegL, 0xE8);

	RFIDWriteRegister(MFRC_TxASKReg, 0x40);		// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	RFIDWriteRegister(MFRC_ModeReg, 0x3D);		// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)

	RFIDAntennaOn();
}

void RFIDReset(){
	RFIDSendCommand(MFRC_CMD_SOFT_RESET);
	int count = 0;
	do {
		// Wait for the PowerDown bit in CommandReg to be cleared (max 3x50ms)
		HAL_Delay(50);
	} while((RFIDReadRegister(MFRC_CommandReg) & (1 << 4)) && ((++count) < 3));
}

void RFIDAntennaOn(){
	uint8_t value = RFIDReadRegister(MFRC_TxControlReg);
	if((value & 0x03) != 0x03){
		RFIDWriteRegister(MFRC_TxControlReg, value | 0x03);
	}

}

void RFIDAntennaOff(){
	uint8_t value = RFIDReadRegister(MFRC_TxControlReg);
	RFIDWriteRegister(MFRC_TxControlReg, value & ~(0x03))
}

uint8_t RFIDGetAntennaGain(){
	return RFIDReadRegister(MFRC_RFCfgReg) & (0x07<<4);
}

uint8_t RFIDRequest(uint8_t cmd, uint8_t *response) {
    RFIDWriteRegister(MFRC_ComIrqReg, 0x7F);            // Clear all interrupt flags
    RFIDWriteRegister(MFRC_CommandReg, MFRC_CMD_IDLE);  // Enter idle mode
    RFIDWriteRegister(MFRC_FIFOLevelReg, 0x80);         // Flush FIFO

    RFIDWriteRegister(MFRC_FIFODATAReg, cmd);           // Load REQA (usually 0x26)

    RFIDWriteRegister(MFRC_BitFramingReg, 0x87);        // Tx last 7 bits, StartSend = 1
    RFIDWriteRegister(MFRC_CommandReg, MFRC_CMD_TRANSCEIVE);

    // Wait for RxIRq (bit 5) or timeout (bit 4)
    int count = 0;
    while (!(RFIDReadRegister(MFRC_ComIrqReg) & 0x30) && count++ < 2000);

    // Check for errors
    uint8_t error = RFIDReadRegister(MFRC_ErrorReg);
    if (error & 0x1B) return 0;  // Some error occurred

    // Read the response from FIFO
    uint8_t len = RFIDReadRegister(MFRC_FIFOLevelReg);
    for (int i = 0; i < len; i++) {
        response[i] = RFIDReadRegister(MFRC_FIFODATAReg);
    }

    return len; // Return number of bytes received
}

