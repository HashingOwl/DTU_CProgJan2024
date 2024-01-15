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
#define WIDTH 128
#define HEIGHT 64

extern const uint8_t Bullet_1[1*2];
extern const uint8_t Bullet_2[1*2];
extern const uint8_t Bullet_3[1*2];
extern const uint8_t Bullet_4[1*2];

// Animation
extern const uint8_t* Bullet_Anim[3];

extern const uint8_t Alien1_1[3*4];
extern const uint8_t Alien1_2[3*4];
const uint8_t* Alien1_Anim[2];

extern const uint8_t Alien1_Dead[3*4];

extern const uint8_t TestBG[WIDTH*HEIGHT/4];

#endif /* GRAPHICDATA_H_ */
