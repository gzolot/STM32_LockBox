#include "MFRC522.h"

uint8_t RFIDReadRegister(MFRC_Register address){
	uint8_t tx_buffer[2] = {address | 0x80, 0x00};
	uint8_t rx_buffer[2];
	sendBytes(tx_buffer, 2, rx_buffer);
	return rx_buffer[1];
}


void RFIDWriteRegister(MMFRC_Register address, uint8_t tx_byte){
	uint8_t tx_buffer[2] = {(address << 1) & 0x7E, tx_byte};
	uint8_t rx_buffer[2];
	sendBytes(tx_buffer, 2, rx_buffer);
}
