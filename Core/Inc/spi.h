#include "stm32l476xx.h"
#include <stdint.h>

#define MOSI_GPIO GPIOB
#define MISO_GPIO GPIOB
#define NSS_GPIO GPIOB
#define SCK_GPIO GPIOB
#define MOSI_OFFSET 15
#define MISO_OFFSET 14
#define NSS_OFFSET 12
#define SCK_OFFSET 13

#define SPI_FREQ 2000000 	//2 MHz
#define APB1_FREQ 4000000	//4 MHz

void spiInit(void);
uint8_t spiSendByte(uint8_t);
void spiSendBytes(uint8_t *, int, uint8_t *);
uint8_t spiReadByte(uint8_t);
void spiReadBytes(int, uint8_t *);


