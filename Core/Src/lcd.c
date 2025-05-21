#include "lcd.h"

void lcdInit(){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

		// Initialize PA0–PA10 (D0–D7, RS, RW, E) as output pins
		for (int i = 0; i < 11; i++){
			if (i == 2 || i == 3){
				LCD_GPIO_D2_D3->MODER &= ~(0b11 << (i * 2));
				LCD_GPIO_D2_D3->MODER |= (0b01 << (i * 2));
				LCD_GPIO_D2_D3->OTYPER &=  ~(1 << i);
				continue;
			}
			LCD_GPIO->MODER &= ~(0b11 << (i * 2));
			LCD_GPIO->MODER |= (0b01 << (i * 2));
			LCD_GPIO->OTYPER &=  ~(1 << i);
		}
}

void TIM2Init(){
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	TIM2->CR1 |= TIM_CR1_ARPE;   	//AAR buffer registerd
	TIM2->CR1 &= ~TIM_CR1_UIFREMAP; //no UIF bit
	TIM2->CR1 &= ~TIM_CR1_DIR;		//upcounting
	TIM2->ARR = 0xFFFFFFFF;
	TIM2->PSC = ((APB1_FREQ / TIM2_FREQ) - 1);
	TIM2->CR1 |= TIM_CR1_CEN;
}

void DelayUS(uint32_t us) {

	uint32_t start = TIM2->CNT; // Start timer
	uint32_t duration = us * (TIM2_FREQ/1000000);
	while ((TIM2->CNT - start) < duration); // Wait until timer reaches target
}

void writeInstruction(uint8_t data){
	LCD_GPIO->ODR &= ~(1<<RS_OFFSET);
	LCD_GPIO->ODR &= ~(1<<RW_OFFSET);
	LCD_GPIO->ODR = (LCD_GPIO->ODR & ~LCD_DATA_MASK) | (data & LCD_DATA_MASK);
	LCD_GPIO_D2_D3->ODR = (LCD_GPIO_D2_D3->ODR & ~LCD_D2_D3_MASK) | (data & LCD_D2_D3_MASK);
	LCD_GPIO->ODR |= (1<<E_OFFSET);
	DelayUS(100);
	LCD_GPIO->ODR &= ~(1<<E_OFFSET);
	DelayUS(100);
}

void lcdClear() {
    writeInstruction(0x01);
    DelayUS(2000);  // Clear takes ~1.52ms
}

void lcdReturnHome() {
    writeInstruction(0x02);
    DelayUS(2000);  // Same as clear display
}


//Entry Mode Set (0x06 = Increment, No Shift)
void lcdEntryModeSet() {
    writeInstruction(0x06);
    DelayUS(100);
}

//Display ON, Cursor OFF, Blink OFF (0x0C)
void lcdDisplayOn() {
    writeInstruction(0x0C);
    DelayUS(100);
}

//Function Set: 8-bit, 2-line, 5x8 font (0x38)
void lcdFunctionSet() {
    writeInstruction(0x38);
    DelayUS(100);
}

//Set DDRAM Address (cursor position)
void lcdSetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = col + (row == 1 ? 0x40 : 0x00);
    writeInstruction(0x80 | addr);
    DelayUS(100);
}

void lcdBegin() {
    DelayUS(15000);      // Wait >15ms after VCC rises to 4.5V
    lcdFunctionSet();    // 8-bit, 2-line, 5x8 font
    DelayUS(4100);       // Wait >4.1ms
    lcdFunctionSet();    // Repeat
    DelayUS(100);        // Wait >100µs
    lcdFunctionSet();    // Confirm
    lcdDisplayOn();      // Display ON, Cursor OFF, Blink OFF
    lcdClear();          // Clear display
    lcdEntryModeSet();   // Entry mode: increment, no shift
}

void writeData(uint8_t data){
	LCD_GPIO->ODR |= (1<<RS_OFFSET);
	LCD_GPIO->ODR &= ~(1<<RW_OFFSET);
	LCD_GPIO->ODR = (LCD_GPIO->ODR & ~LCD_DATA_MASK) | (data & LCD_DATA_MASK);
	LCD_GPIO_D2_D3->ODR = (LCD_GPIO_D2_D3->ODR & ~LCD_D2_D3_MASK) | (data & LCD_D2_D3_MASK);
	LCD_GPIO->ODR |= (1<<E_OFFSET);
	DelayUS(100);
	LCD_GPIO->ODR &= ~(1<<E_OFFSET);
	DelayUS(100);
}

void lcdWriteChar(char c) {
    if (c >= 0x20 && c <= 0x7E) {
        writeData((uint8_t)c);  // Function that sets RS=1 and sends data to LCD
    } else {
        writeData(' ');  // fallback to space for unsupported chars
    }
}

void lcdWriteString(char *word, int len){
	for (int i = 0; i < len; i ++){
		lcdWriteChar(word[i]);
	}
}



