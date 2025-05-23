#include "spi.h"

void spiInit(){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;

	//enable alternate function mode for each GPIO
	MOSI_GPIO->MODER &= ~(0b11 << (MOSI_OFFSET * 2));
	MOSI_GPIO->MODER |=  (0b10 << (MOSI_OFFSET * 2));  	// Alternate function mode

	MISO_GPIO->MODER &= ~(0b11 << (MISO_OFFSET * 2));
	MISO_GPIO->MODER |=  (0b10 << (MISO_OFFSET * 2));  	// Alternate function mode

	SCK_GPIO->MODER  &= ~(0b11 << (SCK_OFFSET * 2));
	SCK_GPIO->MODER  |=  (0b10 << (SCK_OFFSET * 2));   	// Alternate function mode

	NSS_GPIO->MODER  &= ~(0b11 << (NSS_OFFSET * 2));
	NSS_GPIO->MODER  |=  (0b10 << (NSS_OFFSET * 2));   	// Alternate function mode

	// Set speed for GPIO
	MOSI_GPIO->OSPEEDR |= (0b11 <<(MOSI_OFFSET * 2));	// Very high speed

	SCK_GPIO->OSPEEDR |= (0b11 <<(SCK_OFFSET * 2));		// Very high speed

	NSS_GPIO->OSPEEDR |= (0b11 <<(NSS_OFFSET * 2));		// Very high speed

	MISO_GPIO->OSPEEDR |= (0b11 << (MISO_OFFSET * 2)); 	// Very high speed

	// AFR: Set alternate function 5 (AF5 = SPI2 on PB12–PB15)
	MOSI_GPIO->AFR[1] &= ~(0xF << ((MOSI_OFFSET % 8) * 4));
	MOSI_GPIO->AFR[1] |= (0x5 << ((MOSI_OFFSET % 8) * 4));

	SCK_GPIO->AFR[1] &= ~(0xF << ((SCK_OFFSET % 8) * 4));
	SCK_GPIO->AFR[1] |= (0x5 << ((SCK_OFFSET % 8) * 4));

	MISO_GPIO->AFR[1] &= ~(0xF << ((MISO_OFFSET % 8) * 4));
	MISO_GPIO->AFR[1] |= (0x5 << ((MISO_OFFSET % 8) * 4));

	NSS_GPIO->AFR[1] &= ~(0xF << ((NSS_OFFSET % 8) * 4));
	NSS_GPIO->AFR[1] |= (0x5 << ((NSS_OFFSET % 8) * 4));

	SPI2->CR1 =
}
