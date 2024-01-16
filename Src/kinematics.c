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
void applyGravity(GravityTarget *body, GravitySource objects[], uint8_t numOfObj){
	// Iterate through each object
	for (uint8_t i = 0; i < numOfObj; ++i){
		//Squared Distance from body to object. An offset is added to prevent VERY large numbers when close.
		uint32_t distanceSquared = distFIXSquared(&body->pos, &objects[i].pos, 10) + 0x400;
		if(distanceSquared < (0x6200 * 0x6200) >> 10){ //Distance less than 50
			int32_t gravForceMagnitude = objects[i].mass / distanceSquared;

			// Vector from body to the object. Gives the direction of gravity
			vector_t forceVector = subtractVectors(&(objects[i].pos), &(body -> pos));

			//Now forceVector becomes the actual force applied on the body.
			forceVector.x *= gravForceMagnitude;
			forceVector.y *= gravForceMagnitude;

			//The force is scaled down significantly
			forceVector.x >>= 26;
			forceVector.y >>= 26;

			// Update body's velocity
			body->vel.x += forceVector.x;
			body->vel.y += forceVector.y;

			// Det ser ud til at der ikke tages højde for masse når legemet acceleres af kraften.
			// Dette resultere i at tunge objekter falder langsomere end lette objekter, hvilket ikke er ønskværdigt
		}
	}
}

//Returns true if the sqaured distance between vectors p and q are less than radius
char circleCollision(vector_t *p, vector_t *q, uint32_t *squareDistance){
	uint32_t dist = distFIXSquared(p, q, FIX);
	return dist < *squareDistance;
}

//Returns true if the vector pos collides with any of the objects
char checkCollisions(vector_t *pos, GravitySource sources[], uint8_t numOfSources){
	for(uint8_t i = 0; i < numOfSources; i++){
		if(circleCollision(pos, &(sources[i].pos), &(sources[i].squareRadius))){
			return 1;
		}
	}
	return 0;
}
