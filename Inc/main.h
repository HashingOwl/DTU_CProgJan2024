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
	vector_t pos, anchor;
	uint8_t power;
	uint16_t framesLeft;
	char isActive;
}powerup;

typedef struct {
	vector_t orbitPos, pos, anchor;
	int32_t orbitRadius, radius;
	int phase; // 512 in a rotation
} sentry_t;

typedef struct {
	vector_t pos, vel, anchor;
	uint32_t radius;
	char isActive;
}bullet;


//----------------------------------GAME CONTROL----------------------------
void initGame(const uint8_t* currentBackground, GravitySource asteroids[], uint8_t numAsteroids, uint8_t livesLeft);

//----------------------------------PLAYER CONTROL----------------------------
void shipUpdatePosition(GravityTarget *ship);
void makeNewAlien(GravityTarget *ship, vector_t *startPos, uint8_t* currentAlien);

//-----------------------------------BULLETS------------------------------------------------------
void bulletUpdatePosition(bullet bullets[], uint8_t numOfBullets, GravitySource asteroids[], uint8_t numAsteroids);

char bulletHitPlayer(vector_t* playerPos, bullet bullets[], uint8_t numBullets);
char sentryHitPlayer(GravityTarget* ship, sentry_t sentries[], uint8_t numSentries);
void drawAllBullets(bullet bullets[], uint8_t numOfBullets, uint32_t frameCount, const uint8_t* background);
void generateBullets(bullet bullets[], uint8_t numOfBullets, sentry_t* enemies, uint8_t numOfEnemies, vector_t *playerPos, uint16_t bulletSpeed);
int16_t getBulletCountdown();
//----------------------------------POWERUPS-----------------------------------
int16_t getPowerupCountdown();
void generateNewPowerup(powerup powerups[], uint8_t numPowerups, uint16_t frameCount, const uint8_t currentbackground[], GravitySource asteroids[], uint8_t numAsteroids);

//--------------------------------DRAWING FUNCITONS---------------------------------------------

void drawBullet(bullet* bullet, uint32_t frameCount, const uint8_t* background);

void drawAsteroid(GravitySource* asteroid, const uint8_t* background);

void drawAlien(GravityTarget* alien, uint8_t alienNum, uint8_t* powerUpEffects, uint32_t frameCount, const uint8_t* background, uint8_t playerHit);

void updateSentryPos(sentry_t* sentry, uint32_t frameCount);

void drawSentry(sentry_t* sentry, uint32_t frameCount, const uint8_t* background);

void drawPowerup(powerup* powerup, uint32_t frameCount, const uint8_t* background);

//----------------------------------LED-----------------------------------------
void setLEDToIndicateHealth(uint8_t livesLeft);

//----------------------------------FRAME TIMER---------------------------------
void TIM1_BRK_TIM15_IRQHandler(void);

void initTimer15(uint16_t prescale, uint32_t reloadValue);

//This interrupt handles BossScreen.
void EXTI4_IRQHandler(void);

void initBossScreen(void);



#endif /* MAIN_H_ */
