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
	uint8_t power;
	uint16_t framesLeft;
	char isActive;
}powerup;

typedef struct {
	vector_t pos, vel, anchor;
	uint32_t radius;
	char isActive;
}bullet;

//DISSE SKAL RYKKES TIL EN ANDEN FIL
void TIM1_BRK_TIM15_IRQHandler(void);

void initTimer15(uint16_t prescale, uint32_t reloadValue);

void bulletUpdatePosition(bullet bullets[], uint8_t numOfBullets, GravitySource asteroids[], uint8_t numAsteroids);
char bulletHitPlayer(vector_t* playerPos, bullet bullets[], uint8_t numBullets);

void shipUpdatePosition(GravityTarget *ship);

int16_t getPowerupCountdown();
void generateNewPowerup(powerup powerups[], uint8_t numPowerups);

void drawBullet(bullet* bullet, uint32_t frameCount, const uint8_t* background);
void drawAllBullets(bullet bullets[], uint8_t numOfBullets, uint32_t frameCount, const uint8_t* background);
void generateBullets(bullet bullets[], uint8_t numOfBullets, vector_t enemies[], uint8_t numOfEnemies, vector_t *playerPos, uint16_t bulletSpeed);

void drawAsteroid(GravitySource* asteroid, const uint8_t* background);
void makeNewAlien(GravityTarget *ship, vector_t *startPos, uint8_t* currentAlien);

void setLEDToIndicateHealth(uint8_t livesLeft);

void drawAlien(GravityTarget* alien, int alienNum, uint32_t frameCount, const uint8_t* background, uint8_t playerHit);

void initBossScreen(void);
#endif /* MAIN_H_ */
