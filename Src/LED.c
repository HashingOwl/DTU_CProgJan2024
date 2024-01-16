#include "stm32f30x_conf.h"

void initLED (void) {
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB;	// Enable clock for GPIO Port B
	RCC->AHBENR |= RCC_AHBPeriph_GPIOC;	// Enable clock for GPIO Port C

	GPIOA->OSPEEDR &= ~(0x00000003 << (9 * 2)); // Clear speed register
	GPIOA->OSPEEDR |= (0x00000002 << (9 * 2)); // set speed register (0x01 - 10
	//MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOA->OTYPER &= ~(0x0001 << (9 * 1)); // Clear output type register
	GPIOA->OTYPER |= (0x0000 << (9)); // Set output type register (0x00 -
	//Push pull, 0x01 - Open drain)
	GPIOA->MODER &= ~(0x00000003 << (9 * 2)); // Clear mode register
	GPIOA->MODER |= (0x00000001 << (9 * 2)); // Set mode register (0x00 –
	//Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	//GPIOA->ODR |= (0x0001 << 9); //Set pin PA1 to high

	GPIOB->OSPEEDR &= ~(0x00000003 << (4 * 2)); // Clear speed register
	GPIOB->OSPEEDR |= (0x00000002 << (4 * 2)); // set speed register (0x01 - 10
	GPIOB->OTYPER &= ~(0x0001 << (4 * 1)); // Clear output type register
	GPIOB->OTYPER |= (0x0000 << (4)); // Set output type register (0x00 -
	GPIOB->MODER &= ~(0x00000003 << (4 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000001 << (4 * 2)); // Set mode register (0x00 –

	GPIOC->OSPEEDR &= ~(0x00000003 << (7 * 2)); // Clear speed register
	GPIOC->OSPEEDR |= (0x00000002 << (7 * 2)); // set speed register (0x01 - 10
	GPIOC->OTYPER &= ~(0x0001 << (7 * 1)); // Clear output type register
	GPIOC->OTYPER |= (0x0000 << (7)); // Set output type register (0x00 -
	GPIOC->MODER &= ~(0x00000003 << (7 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000001 << (7 * 2)); // Set mode register (0x00 –
}

/*
Color 0: off
Color 1: Red
Color 2: Green
Color 3: Yellow
Color 4: Blue
Color 5: Pink
Color 6: Cyan
Color 7: White*/
void setLED(uint8_t color) {
	GPIOB->ODR |= (0x0001 << 4);
	GPIOC->ODR |= (0x0001 << 7);
	GPIOA->ODR |= (0x0001 << 9);
	if (color & 0x0001){
		GPIOB->ODR &= ~(0x0001 << 4);
	}
	if (color & 0x0002){
		GPIOC->ODR &= ~(0x0001 << 7);
	}
	if (color & 0x0004){
		GPIOA->ODR &= ~(0x0001 << 9);
	}
}


