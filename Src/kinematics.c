/*
 * kinematics.c
 *
 *  Created on: Jan 9, 2024
 *      Author: rotho
 */
#include <stdio.h>
#include "math.h"
#include "kinematics.h"


//Aplies linear gravity (F=G*M/r) from all objects to body
void applyGravity(Body *body, Object objects[], uint8_t numOfObj){
	// Iterate through each object
	for (uint8_t i = 0; i < numOfObj; ++i){
		//Squared Distance from body to object. An offset is added to prevent VERY large numbers when close.
		uint32_t distanceSquared = distFIXSquared(&body->pos, &objects[i].pos, 10) + 0x400;
		if(distanceSquared < 0x3200 * 0x3200){ //Distance less than 50
			int32_t gravForceMagnitude = objects[i].mass / distanceSquared;

			// Vector from body to the object. Gives the direction of gravity
			vector_t forceVector = subtractVectors(&(objects[i].pos), &(body -> pos));

			//Now forceVector becomes the actual force applied on the body.
			forceVector.x *= gravForceMagnitude;
			forceVector.y *= gravForceMagnitude;

			//The force is scaled down significantly
			forceVector.x >>= 25;
			forceVector.y >>= 25;

			// Update body's velocity
			body->vel.x += forceVector.x;
			body->vel.y += forceVector.y;
		}
	}
}

//Returns true if the sqaured distance between vectors p and q are less than radius
char circleCollision(vector_t *p, vector_t *q, uint32_t *radius){
	return distFIXSquared(p, q, FIX) < *radius;
}

//Returns true if the vector pos collides with any of the objects
char checkCollisions(vector_t *pos, Object objects[], uint8_t numOfObj){
	for(uint8_t i = 0; i < numOfObj; i++){
		if(circleCollision(pos, &(objects[i].pos), &(objects[i].radius))){
			return 1;
		}
	}
	return 0;
}
