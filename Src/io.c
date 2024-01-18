/*
 * io.c
 *
 *  Created on: Jan 5, 2024
 *      Author: rotho
 */
#include <io.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#define RIGHT_JOY1 GPIOC, 0
#define UP_JOY1 GPIOA, 4
#define CENTER_JOY1 GPIOB, 5
#define LEFT_JOY1 GPIOC, 1
#define DOWN_JOY1 GPIOB, 0

#define LED_R GPIOB, 4
#define LED_G GPIOC, 7
#define LED_B GPIOA, 9

void setPinToInput(GPIO_TypeDef *port, unsigned char pin){
	port-> MODER &= ~(0x00000003 << (pin * 2)); // Clear mode register
	//port-> MODER |= (0x00000000 << (pin * 2)); // Set mode register (0x00 – Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)

	port->PUPDR &= ~(0x00000003 << (pin * 2)); // Clear push/pull register
	port->PUPDR |= (0x00000002 << (pin * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

}

void setPinToOutput(GPIO_TypeDef *port, unsigned char pin){
	port -> OSPEEDR &= ~(0x3 << (pin * 2)); // Clear speed register
	port -> OSPEEDR |= (0x2 << (pin * 2)); // set speed register (0x01 - 10MHz, 0x02 - 2MHz, 0x03 - 50MHz)

	port -> OTYPER &= ~(0x1 << (pin * 1)); // Clear output type register
	//port -> OTYPER |= (0x0000 << (pin)); // Set output type register (0x00 - Push pull, 0x01 - Open drain)

	port -> MODER &= ~(0x00000003 << (pin * 2)); // Clear mode register
	port -> MODER |= (0x00000001 << (pin * 2)); // Set mode register (0x00 –Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
}

void setPinValue(GPIO_TypeDef *port, uint8_t pin, uint8_t value){
	if(value)
		port -> ODR |= (0x0001 << pin);
	else
		port -> ODR &= ~(0x0001 << pin);
}
void setPort(GPIO_TypeDef *port, uint8_t pins, uint8_t value){
	port -> ODR &= ~pins; 	//Setting selected pins in register to zero
	value &= pins; 			//Masking value with selected pins
	port -> ODR |= value;	//Setting the pins in the array
}

void initDigitalJoystick(){
	RCC-> AHBENR |= RCC_AHBPeriph_GPIOA;
	RCC-> AHBENR |= RCC_AHBPeriph_GPIOB;
	RCC-> AHBENR |= RCC_AHBPeriph_GPIOC;

	setPinToInput(GPIOA, 4);
	setPinToInput(GPIOB, 0);
	setPinToInput(GPIOB, 5);
	setPinToInput(GPIOC, 0);
	setPinToInput(GPIOC, 1);
}

void initLED(){
	//Enable clock for GPIO ports A, B and C
	RCC-> AHBENR |= RCC_AHBPeriph_GPIOA;
	RCC-> AHBENR |= RCC_AHBPeriph_GPIOB;
	RCC-> AHBENR |= RCC_AHBPeriph_GPIOC;

	setPinToOutput(LED_R);
	setPinToOutput(LED_G);
	setPinToOutput(LED_B);

	//Disable LED
	setPinValue(LED_R, 1);
	setPinValue(LED_G, 1);
	setPinValue(LED_B, 1);
}

uint16_t readPort(GPIO_TypeDef *port, uint16_t pins){
	return port -> IDR & pins;
}

char readSinglePin(GPIO_TypeDef *port, uint8_t pin){
	uint16_t reading = port -> IDR & (1 << pin);
	return (char) (reading >> pin);
}


uint8_t readDigitalJoystick(){
	uint8_t joy = 0;
	joy |= readSinglePin(UP_JOY1) << 0;
	joy |= readSinglePin(DOWN_JOY1) << 1;
	joy |= readSinglePin(LEFT_JOY1) << 2;
	joy |= readSinglePin(RIGHT_JOY1) << 3;
	joy |= readSinglePin(CENTER_JOY1) << 4;

	return joy;
}

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

//Timer
void initTimer15(uint16_t prescale, uint32_t reloadValue){
	RCC->APB2ENR |= RCC_APB2Periph_TIM15;
	TIM15->CR1 &= 0b1111010001110000;
	TIM15->CR1 |= TIM_CR1_CEN;
	TIM15->ARR = reloadValue;
	TIM15->PSC = prescale;

	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts
	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 10); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
}
void resetTimer15(){
	TIM15->SR &= ~0x0001; // Clear interrupt bit
}

