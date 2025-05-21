#include <stdint.h>
#include "stm32l476xx.h"

#define D0_OFFSET 0
#define D1_OFFSET 1
#define D2_OFFSET 2
#define D3_OFFSET 3
#define D4_OFFSET 4
#define D5_OFFSET 5
#define D6_OFFSET 6
#define D7_OFFSET 7
#define LCD_DATA_MASK 0x00F3
#define LCD_D2_D3_MASK 0x000C
#define RS_OFFSET 8
#define RW_OFFSET 9
#define E_OFFSET 10
#define LCD_GPIO GPIOA
#define LCD_GPIO_D2_D3 GPIOB

#define APB1_FREQ 4000000  	// 1 MHz
#define TIM2_FREQ 1000000 	// 4 MHZ


void lcdInit(void);
void TIM2Init(void);
void DelayUS(uint32_t);
void writeInstruction(uint8_t);
void lcdClear(void);
void lcdReturnHome(void);
void lcdEntryModeSet(void);
void lcdDisplayOn(void);
void lcdFunctionSet(void);
void lcdSetCursor(uint8_t, uint8_t);
void lcdWriteChar(char);
void writeData(uint8_t data);
void lcdBegin(void);
void lcdWriteString(char *, int);


