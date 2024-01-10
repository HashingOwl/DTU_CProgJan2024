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

void updatePosition(Body *body);

#endif /* KINEMATICS_H_ */
