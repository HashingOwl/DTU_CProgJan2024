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
void applyGravity(vector_t pos, vector_t* vel, GravitySource objects[], uint8_t numOfObj){
	// Iterate through each object
	for (uint8_t i = 0; i < numOfObj; ++i){
		//Squared Distance from body to object. An offset is added to prevent VERY large numbers when close.
		uint32_t distanceSquared = distFIXSquared(&pos, &objects[i].pos, 8) + 0x400;
		if(distanceSquared < (0x6200 * 0x6200) >> 8){ //Distance less than 50
			int32_t gravForceMagnitude = objects[i].mass / distanceSquared;

			// Vector from body to the object. Gives the direction of gravity
			vector_t forceVector = subtractVectors(&(objects[i].pos), &pos);

			//Now forceVector becomes the actual force applied on the body.
			forceVector.x *= gravForceMagnitude;
			forceVector.y *= gravForceMagnitude;

			//The force is scaled down significantly
			forceVector.x >>= 23;
			forceVector.y >>= 23;

			// Update body's velocity
			vel-> x += forceVector.x;
			vel->y += forceVector.y;
		}
	}
}


//Returns true if the sqaured distance between vectors p and q are less than radius
char circleCollision(vector_t *p, vector_t *q, uint32_t squareDistance){
	uint32_t dist = distFIXSquared(p, q, FIX);
	return dist < squareDistance;
}

//Returns true if the vector pos collides with any of the objects
/*
char checkCollisions(GravityTarget* target, GravitySource sources[], uint8_t numOfSources){
	for(uint8_t i = 0; i < numOfSources; i++){
		if(circleCollision(&(target->pos), &(sources[i].pos), target->squareRadius + sources[i].squareRadius)){
			return 1;
		}
	}
	return 0;
}
*/
