/*
 * main.h
 *
 *  Created on: 14. jan. 2024
 *      Author: rotho
 */
#include "math.h"

#ifndef MAIN_H_
#define MAIN_H_

enum powerupEnum {health, speed};

typedef struct {
	vector_t pos;
}powerup;

typedef struct {
	vector_t pos, vel, anchor;
	char isActive;
}bullet;

//DISSE SKAL RYKKES TIL EN ANDEN FIL
void TIM1_BRK_TIM15_IRQHandler(void);

void initTimer15(uint16_t prescale, uint32_t reloadValue);

void bulletUpdatePosition(GravityTarget bullets[], uint8_t numOfBullets);

void shipUpdatePosition(GravityTarget *ship);

inline int16_t getPowerupCountdown();

void drawBullet(bullet* bullet, uint32_t frameCount, const uint8_t* background);
void drawAllBullets(bullet bullets[], uint8_t numOfBullets, vector_t* offset, uint32_t frameCount, uint8_t* background);
void generateBullets(bullet bullets[], uint8_t numOfBullets, vector_t enemies[], uint8_t numOfEnemies, vector_t *playerPos);

void drawAsteroid(GravitySource* asteroid, const uint8_t* background);

void drawAlien(GravityTarget* alien, int alienNum, uint32_t frameCount, const uint8_t* background);

#endif /* MAIN_H_ */
