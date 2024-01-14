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
	char draw;
}Body;

typedef struct {
	vector_t pos;
	uint32_t radius;
	uint32_t mass;
}Object;

//Aplies linear gravity (F=G*M/r) from all objects to body
void applyGravity(Body *body, Object objects[], uint8_t numOfObj);

//Returns true if the sqaured distance between vectors p and q are less than radius
char circleCollision(vector_t *p, vector_t *q, uint32_t *radius);

//Returns true if the vector pos collides with any of the objects
char checkCollisions(vector_t *pos, Object objects[], uint8_t numOfObj);

#endif /* KINEMATICS_H_ */
