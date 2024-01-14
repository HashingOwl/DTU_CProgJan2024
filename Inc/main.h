/*
 * main.h
 *
 *  Created on: 14. jan. 2024
 *      Author: rotho
 */
#include "math.h"

#ifndef MAIN_H_
#define MAIN_H_

void bulletUpdatePosition(Body bullets[], uint8_t numOfBullets);
vector8_t analogJoystickInput();
void initTimer();

#endif /* MAIN_H_ */
