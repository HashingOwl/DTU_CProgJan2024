/*
 * GraphicData.h
 *
 *  Created on: 8. jan. 2024
 *      Author: valde
 */

#ifndef GRAPHICDATA_H_
#define GRAPHICDATA_H_

#include <stdint.h>

// In PuTTY coordinates
#define P_WIDTH 200
#define P_HEIGHT 100
/*
 * GraphicData.c
 *
 *  Created on: 8. jan. 2024
 *      Author: valde
 */

#include "GraphicData.h"

//==========================================SPRITES==============================================

//------Bullet------
extern const uint8_t Bullet_1[1*2];
extern const uint8_t Bullet_2[1*2];
extern const uint8_t Bullet_3[1*2];

//-----Asteroid-----
extern const uint8_t Asteroid_1[5*10];

//------Power-Ups----
extern const uint8_t PU_Anti_Gravity[18];
extern const uint8_t PU_Heart[18];
extern const uint8_t PU_Speed[18];
extern const uint8_t PU_Shield[18];

//------Sentry------
extern const uint8_t Sentry_1[5*4];
extern const uint8_t Sentry_2[5*4];
extern const uint8_t Sentry_3[5*4];

//----EmptyAlien----
extern const uint8_t emptyAlien[3*6];

//------Alien1------
extern const uint8_t Alien1_1[3*6];
extern const uint8_t Alien1_2[3*6];
extern const uint8_t Alien1_Dead[3*6];

//------Alien2------
extern const uint8_t Alien2_1[3*6];
extern const uint8_t Alien2_2[3*6];
extern const uint8_t Alien2_Dead[3*6];

//------Alien3------
extern const uint8_t Alien3_1[2*4];
extern const uint8_t Alien3_2[2*4];
extern const uint8_t Alien3_Dead[2*4];

//========================================ANIMATIONS=============================================

extern const uint8_t* Alien1_Anim[2];
extern const uint8_t* Alien2_Anim[2];
extern const uint8_t* Alien3_Anim[2];
extern const uint8_t* Bullet_Anim[3];
extern const uint8_t* Sentry_Anim[4];

//========================================BACKGROUNDS============================================
extern const uint8_t BG_Stratosphere_1[128*64/4];
extern const uint8_t TestBG[128*64/4];
extern const uint8_t BG_Stratosphere_2[200*200/8];
extern const uint8_t bossScreenBG[200*200/8];
extern const uint8_t MainMenuBG[5000];
const uint8_t HelpScreen[5000];
#endif /* GRAPHICDATA_H_ */
