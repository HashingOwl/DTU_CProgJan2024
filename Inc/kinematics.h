/*
 * kinematics.h
 *
 *  Created on: Jan 9, 2024
 *      Author: rotho
 */
#include "math.h"

#ifndef KINEMATICS_H_
#define KINEMATICS_H_

// "anchor" is for graphics, since every gravityTarget and gravitySource is renderes graphicly
// "anchor" is not in fixedPoint, but just regular intergers, as it is only used for graphics. int32_t is overkill.
typedef struct {
	vector_t pos, vel, anchor;
	uint32_t radius;
	char isActive;
} GravityTarget;

typedef struct {
	vector_t pos, anchor;
	uint32_t radius;
	uint32_t mass;
} GravitySource;

//Aplies linear gravity (F=G*M/r) from all sources to the target
void applyGravity(vector_t pos, vector_t* vel, GravitySource objects[], uint8_t numOfObj);

//Returns true if the sqaured distance between vectors p and q are less than squareRadius
char circleCollision(vector_t *p, vector_t *q, uint32_t squareDistance);

//Returns true if the vector pos collides with any of the objects
//char checkCollisions(GravityTarget* target, GravitySource sources[], uint8_t numOfsources);

#endif /* KINEMATICS_H_ */
