/*
 * io.h
 *
 *  Created on: Jan 5, 2024
 *      Author: rotho
 */
#include <stdio.h>

void initDigitalJoystick();
void initLED();
uint8_t readDigitalJoystick();
void setLED(uint8_t color);
void resetTimer15();

