/*
 * Joystick.c
 *
 *  Created on: 9. jan. 2024
 *      Author: alber
 */

#include "stm32f30x_conf.h" // STM32 config

/*
 Ting at forbedre:
 -Brug interupts i stedet for at vente
 -Lav adjustable pins
 */

void initJoystickAnalog () {
	//Initializes PA4 as Joystick-X and PA6 as Joysticj-Y
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A

	GPIOA->MODER |= (0x00000000 << (6 * 2));
	GPIOA->PUPDR &= ~(0x00000003 << (6 * 2));
	GPIOA->PUPDR |= (0x00000002 << (6 * 2));

	GPIOA->MODER |= (0x00000000 << (4 * 2));
	GPIOA->PUPDR &= ~(0x00000003 << (4 * 2));
	GPIOA->PUPDR |= (0x00000002 << (4 * 2));

	//Initializes ADC1
	RCC->CFGR2 &= ~RCC_CFGR2_ADCPRE12; // Clear ADC12 prescaler bits
	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6; // Set ADC12 prescaler to 6
	RCC->AHBENR |= RCC_AHBPeriph_ADC12; // Enable clock for ADC12

	ADC1->CR = 0x00000000; // Clear CR register
	ADC1->CFGR &= 0xFDFFC007; // Clear ADC1 config register
	ADC1->SQR1 &= ~ADC_SQR1_L; // Clear regular sequence register 1

	ADC1->CR |= 0x10000000; // Enable internal ADC voltage regulator
	for (int i = 0 ; i < 1000 ; i++) {} // Wait for about 16 microseconds

	ADC1->CR |= 0x80000000; // Start ADC1 calibration
	while (!(ADC1->CR & 0x80000000)); // Wait for calibration to finish
	for (int i = 0 ; i < 100 ; i++) {} // Wait for a little while

	ADC1->CR |= 0x00000001; // Enable ADC1 (0x01 - Enable, 0x02 - Disable)
	while (!(ADC1->ISR & 0x00000001)); // Wait until ready
}

uint16_t JoystickXADC() {
	//Reads ADC value on PA4 (Joy-X)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_1Cycles5);
	ADC_StartConversion(ADC1); // Start ADC read
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
	return ADC_GetConversionValue(ADC1);
}

uint16_t JoystickYADC() {
	//Reads ADC value on PA6 (Joy-Y)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);
	ADC_StartConversion(ADC1); // Start ADC read
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
	return 4095-ADC_GetConversionValue(ADC1);
}

uint8_t readJoystickDigital() {
	uint8_t res = 0;
	uint16_t adcVal;

	adcVal = JoystickXADC();

	//Checks result and converts it to standard joystick format
	if (adcVal <= 500) {
		res |= 0x04;
	}
	else if (adcVal >= 2000) {
		res |= 0x08;
	}


	adcVal = JoystickYADC();
	//Checks result and converts it to standard joystick format
	if (adcVal <= 500) {
		res |= 0x01;
	}
	else if (adcVal >= 2000) {
		res |= 0x02;
	}

	return res;
}

void readJoystickAnalog(uint32_t* x,uint32_t* y) {
	int32_t adcVal;

	adcVal = JoystickXADC();

	//Create dead-zone and center
	if (1400>adcVal && adcVal>1050) {
		adcVal = 2048;
	}
	adcVal -= 2048;
	adcVal >>= 6;
	(*x) = adcVal;

	adcVal = JoystickYADC();

	//Create dead-zone and center
	if (2100>adcVal && adcVal>1800) {
		adcVal = 2048;
	}
	adcVal -= 2048;
	adcVal >>= 6;
	(*y) = adcVal;

}
