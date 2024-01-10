/*
 * kinematics.c
 *
 *  Created on: Jan 9, 2024
 *      Author: rotho
 */
#include <stdio.h>
#include "math.h"

//Tingene herinde skal ligge i main()
void updatePosition(Body *body, Object *objects[], uint8_t numObj){
	int timeStep = 0x00A0;
	vector_t change = multVector(&((*ball).vel), timeStep);
	vector_t newPos = addVectors(&change, &(ball -> pos));

	if(checkCollisions(body -> pos, objects)){
		body -> vel.x = 0;
		body -> vel.y = 0;
	}
	ball -> pos = newPos;
}

char checkCollisions(vector_t pos, Object *objects[], uint8_t numObj){
	for(uint8_t i = 0; i < numObj; i++){
		if(circleCollision(pos, objects[i]->pos, objects[i]->mass)){
			return 1;
		}
	}
	return 0;
}

char circleCollision(vector_t *p, vector_t *q, int32_t radius){
	return distFIXSquared(p, q) < radius;
}
char rectangleCollision(vector_t *p, int32_t xMinBound, int32_t xMaxBound, int32_t yMinBound, int32_t yMaxBound){
	return p -> x > xMinBound && p -> x < xMaxBound && p -> y > yMinBound && p -> y < yMaxBound;
}

