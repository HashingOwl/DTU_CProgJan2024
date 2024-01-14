#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "math.h"
#include "kinematics.h"
#include "io.h"

//Set true by interrupt when it is time to make a new frame
volatile uint8_t frame = 0;

int main(void)
{
	//Ship and gravityobjects in game
	Body ship = {{2 << FIX, 2<<FIX}, {}};
	uint8_t numOfObj = 3;
	Object objects[numOfObj] = {{.pos = {20 << FIX, 20 << FIX}, .radius = 5 << FIX, .mass = 0x50000000}, {.pos = {40 << FIX, 60 << FIX}, .radius = 5 << FIX, .mass = 0x50000000}, {.pos = {30 << FIX, 40 << FIX}, .radius = 3 << FIX, .mass = 0xAA000000}};
	uint8_t numOfBullets = 20;
	Body bullets[numOfBullets] = {};

	//Console
	int16_t consoleSize = 90;
	int32_t gameSize = consoleSize << FIX;

	void shipUpdatePosition(Body *body, Object objects[], uint8_t numOfObj){
		vector_t newPos = addVectors(&(body->vel), &(body->pos));
		clampVector(&newPos, 1 << FIX, gameSize); //Keep body within bounds

		if(checkCollisions(&newPos, objects, numOfObj)){
			body -> vel.x = 0;
			body -> vel.y = 0;
			//MORE CODE TO HANDLE COLLISION
				//Eg. loose af life. Decrease score. Play animation
		}
		body -> pos = newPos;
	}

	while(1){
		if(frame){
			frame = 0;

			//Update velocity based on input
			vector8_t input = analogJoystickInput();
			ship.vel.x += input.x;
			ship.vel.y += input.y;

			//Remove old graphic. Console coordinates are (ship.x >> FIX, ship.y >> FIX)

			applyGravity(&ship, objects, numOfObj);
			shipUpdatePosition(&ship, objects, numOfObj);
			bulletUpdatePosition(&bullets, numOfBullets);

			//Draw new graphic. Console coordinates are (ship.x >> FIX, ship.y >> FIX)
		}
	}
}

//NOT IMPLEMENTED
vector8_t analogJoystickInput(){
	//Take analog input from 30010 joystick and return it with max value around 0x30
}

void bulletUpdatePosition(Body bullets[], uint8_t numOfBullets){
	for(uint8_t i = 0; i < numOfBullets; i++){
		if(bullets[i].draw){
			bullets[i].pos.x += bullets[i].vel.x;
			bullets[i].pos.y += bullets[i].vel.y;
		}
	}
}
