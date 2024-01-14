/*
 * kinematics.h
 *
 *  Created on: Jan 9, 2024
 *      Author: rotho
 */
#include "math.h"

#ifndef KINEMATICS_H_
#define KINEMATICS_H_

typedef struct {
	vector_t pos, vel;
	char isActive;
}GravityTarget;

typedef struct {
	vector_t pos;
	uint32_t squareRadius;
	uint32_t mass;
}GravitySource;

//Aplies linear gravity (F=G*M/r) from all sources to the target
void applyGravity(GravityTarget *target, GravitySource sources[], uint8_t numOfSoruces);

//Returns true if the sqaured distance between vectors p and q are less than squareRadius
char circleCollision(vector_t *p, vector_t *q, uint32_t *squareDistance);

//Returns true if the vector pos collides with any of the objects
char checkCollisions(vector_t *pos, GravitySource sources[], uint8_t numOfsources);

#endif /* KINEMATICS_H_ */
