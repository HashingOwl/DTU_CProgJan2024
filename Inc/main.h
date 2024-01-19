/*
 * main.h
 *
 *  Created on: 14. jan. 2024
 *      Author: rotho
 */
#include "math.h"
#include "kinematics.h"
#include "graphics.h"

#ifndef MAIN_H_
#define MAIN_H_

// Game states
#define MENU 1
#define PLAYING 2
#define HELP 3

// Fixedpoint unit dimensions
#define BORDER_PAD 	(2 << FIX)
#define U_WIDTH 	(P_WIDTH << FIX)
#define U_HEIGHT 	(P_HEIGHT * 2 << FIX)

typedef struct {
	vector_t pos, anchor;
	uint8_t power;
	uint16_t framesLeft;
	char isActive;
} powerup_t;

typedef struct {
	vector_t orbitPos, pos, anchor;
	int32_t orbitRadius, radius;
	int phase; // 512 in a rotation
} sentry_t;

typedef struct {
	vector_t pos, vel, anchor;
	uint32_t radius;
	char isActive;
} bullet_t;

typedef struct {
	uint8_t numAsteroids, numSentries;
	const GravitySource* asteroidConfiguration;
	const sentry_t* sentryConfiguration;
} map_t;


//----------------------------------GAME CONTROL----------------------------
void initGame(const uint8_t* currentBackground, GravitySource asteroids[], uint8_t numAsteroids, uint8_t livesLeft);

//----------------------------------PLAYER CONTROL----------------------------
void shipUpdatePosition(GravityTarget *ship);
void makeNewAlien(GravityTarget *ship, vector_t *startPos, uint8_t* currentAlien);

//-----------------------------------BULLETS------------------------------------------------------
void bulletUpdatePosition(bullet_t bullets[], uint8_t numOfBullets, GravitySource asteroids[], uint8_t numAsteroids);

char bulletHitPlayer(vector_t* playerPos, bullet_t bullets[], uint8_t numBullets);
char sentryHitPlayer(GravityTarget* ship, sentry_t sentries[], uint8_t numSentries);
void drawAllBullets(bullet_t bullets[], uint8_t numOfBullets, uint32_t frameCount, const uint8_t* background);
void generateBullets(bullet_t bullets[], uint8_t numOfBullets, sentry_t* enemies, uint8_t numOfEnemies, vector_t *playerPos, uint16_t bulletSpeed);
int16_t getBulletCountdown();
//----------------------------------POWERUPS-----------------------------------
int16_t getPowerupCountdown();
void generateNewPowerup(powerup_t powerups[], uint8_t numPowerups, uint16_t frameCount, const uint8_t currentbackground[], GravitySource asteroids[], uint8_t numAsteroids);

//--------------------------------DRAWING FUNCITONS---------------------------------------------

void drawBullet(bullet_t* bullet, uint32_t frameCount, const uint8_t* background);

void drawAsteroid(GravitySource* asteroid, const uint8_t* background);

void drawAlien(GravityTarget* alien, uint8_t alienNum, uint8_t* powerUpEffects, uint32_t frameCount, const uint8_t* background, uint8_t playerHit);

void updateSentryPos(sentry_t* sentry, uint32_t frameCount);

void drawSentry(sentry_t* sentry, uint32_t frameCount, const uint8_t* background);

void drawPowerup(powerup_t* powerup, uint32_t frameCount, const uint8_t* background);

//----------------------------------LED-----------------------------------------
void setLEDToIndicateHealth(uint8_t livesLeft);

//----------------------------------FRAME TIMER---------------------------------
void TIM1_BRK_TIM15_IRQHandler(void);

void initTimer15(uint16_t prescale, uint32_t reloadValue);

//This interrupt handles BossScreen.
void EXTI4_IRQHandler(void);

void initBossScreen(void);



#endif /* MAIN_H_ */
