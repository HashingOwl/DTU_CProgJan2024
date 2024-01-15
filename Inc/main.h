/*
 * main.h
 *
 *  Created on: 14. jan. 2024
 *      Author: rotho
 */
#include "math.h"

#ifndef MAIN_H_
#define MAIN_H_

typedef struct {
	vector_t pos;
}powerup;


//DISSE SKAL RYKKES TIL EN ANDEN FIL
void TIM1_BRK_TIM15_IRQHandler(void);

void initTimer15(uint16_t prescale, uint32_t reloadValue);

void bulletUpdatePosition(GravityTarget bullets[], uint8_t numOfBullets);

void shipUpdatePosition(GravityTarget *target, GravitySource sources[], uint8_t numOfSources);

inline int16_t getPowerupCountdown();

void drawBullet(int x, int y, uint32_t frameCount);

#endif /* MAIN_H_ */
