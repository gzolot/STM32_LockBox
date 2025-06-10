#include "keypad.h"
#include "main.h"

//comment here
char keypadGetKey(const int* col_offsets, int col_len, const int* row_offsets, int row_len, const char keypad_matrix[4][4]){
	int row = keyPressed(row_offsets, row_len);
	if (row == -1){
		return 'F';
	}
	int col = getCol(row, col_offsets, col_len, row_offsets);
	if (col == -1){
		return 'F';
	}
	return keypad_matrix[row][col];

}

void keypadInit(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	//Initialize P0-P3 as output pins
	for (int i = 0; i < 4; i++){
		GPIOC->MODER &= ~(0b11 << (i * 2));
		GPIOC->MODER |= (0b01 << (i * 2));
		GPIOC->OTYPER &=  ~(1 << i);
		GPIOC->ODR |= (1 << i);
	}


	//Initialize P4-P7 as input pins with a pull down
	for (int i = 4; i < 8; i++){
		GPIOC->MODER &= ~(0b11 << (i * 2));
		GPIOC->PUPDR &= ~(0b11 << (i * 2));
		GPIOC->PUPDR |= (0b10 << (i *2));
	}
}

int keyPressed(const int* row_offsets, int rows_len){
	for (int i = 0; i < rows_len; i ++){
		int offset = row_offsets[i];
		if (GPIOC->IDR & (1<<offset)){
			return i;
		}
	}
	return -1;
}

void setCols(const int* col_offsets, int col_len){
	int col_offset;
	for (int i = 0; i < col_len; i++){
			col_offset = col_offsets[i];
			GPIOC->ODR |= (1<<col_offset);
		}
}

void clearCols(const int* col_offsets, int col_len){
	int col_offset;
	for (int i = 0; i < col_len; i++){
			col_offset = col_offsets[i];
			GPIOC->ODR &= ~(1<<col_offset);
		}
}

int getCol(int row_index, const int* col_offsets, int col_len, const int* row_offsets){
	int row_offset = row_offsets[row_index];
	int col_offset;

	clearCols(col_offsets, col_len);

	for (int i = 0; i < col_len; i++){
		col_offset = col_offsets[i];
		GPIOC->ODR |= (1 << col_offset);
		if (GPIOC->IDR & (1<<row_offset)){
			setCols(col_offsets, col_len);
			return i;
		}
	}
	setCols(col_offsets, col_len);
	return -1;
}

