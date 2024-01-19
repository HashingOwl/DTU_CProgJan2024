/*
 * MapData.h
 *
 *  Created on: 19. jan. 2024
 *      Author: valde
 */

#ifndef MAPDATA_H_
#define MAPDATA_H_

#include "main.h"
#include "kinematics.h"

extern const sentry_t Sentry_Configuration_1[4];
extern const GravitySource Asteroid_Configuration_1[7];
extern const map_t Map_1;

//========================================MAP=2=========================================================

extern const sentry_t Sentry_Configuration_2[4];
extern const GravitySource Asteroid_Configuration_2[7];
extern const map_t Map_2;

//========================================MAP=3=========================================================

extern const sentry_t Sentry_Configuration_3[4];
extern const GravitySource Asteroid_Configuration_3[5];
extern const map_t Map_3;

// Maps
extern const map_t* Maps[3];

#endif /* MAPDATA_H_ */
