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
	RFIDWriteRegister(MFRC_TxModeReg, 0x00);
	RFIDWriteRegister(MFRC_RxModeReg, 0x00);
	RFIDWriteRegister(MFRC_ModWidthReg, 0x26);	// Modulation width determines how long each data bit is transmitted via the ASK signal (1.6 µs typical)

	//TODO implement timout potentially if it is necessary
}

void RFIDReset(){
	RFIDSendCommand(MFRC_CMD_SOFT_RESET);
	int count = 0;
	do {
		// Wait for the PowerDown bit in CommandReg to be cleared (max 3x50ms)
		HAL_Delay(50);
	} while((RFIDReadRegister(MFRC_CommandReg) & (1 << 4)) && ((++count) < 3));
}
