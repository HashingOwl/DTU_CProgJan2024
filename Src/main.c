#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "math.h"
#include "kinematics.h"
#include "io.h"

enum joy1Dir {
    joy1UP = 0b1,
    joy1DOWN = 0b10,
    joy1LEFT = 0b100,
    joy1RIGHT = 0b1000,
    joy1CENTER = 0b10000
};


void newline(){
	printf("\n");
}
void gotoxy(int x, int y){
	printf("\033[%d;%dH", y, x);
}


volatile uint8_t frame = 0;
void TIM1_BRK_TIM15_IRQHandler(void){
	frame = 1;
	TIM15->SR &= ~0x0001; // Clear interrupt bit
}

void initTimer(){
	RCC->APB2ENR |= RCC_APB2Periph_TIM15;
	TIM15->CR1 &= 0b1111010001110000;
	TIM15->CR1 |= TIM_CR1_CEN;
	TIM15->ARR = (uint8_t) 50;
	TIM15->PSC = 0xFA00;

	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts
	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 1); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
}

int main(void)
{
	Body ship = {{2 << FIX, 2<<FIX}, {}};
	Object objects[3] = {{.pos = {20 << FIX, 20 << FIX}, .radius = 5 << FIX, .mass = 0x50000000}, {.pos = {40 << FIX, 60 << FIX}, .radius = 5 << FIX, .mass = 0x50000000}, {.pos = {30 << FIX, 40 << FIX}, .radius = 3 << FIX, .mass = 0xAA000000}};
	uint8_t numOfObj = 3;
	int16_t consoleSize = 90;
	int32_t gameSize = consoleSize << FIX;
	uart_init(1000000);
	printf("\033[2J\033[H");
	setupJoystickPins();
	gotoxy(objects[0].pos.x >> FIX, objects[0].pos.y >> FIX);
	printf("O");
	gotoxy(objects[1].pos.x >> FIX, objects[1].pos.y >> FIX);
	printf("O");
	gotoxy(objects[2].pos.x >> FIX, objects[2].pos.y >> FIX);
	printf("O");


	char circleCollision(vector_t *p, vector_t *q, uint32_t *radius){
		return distFIXSquared(p, q, FIX) < *radius;
	}

	char checkCollisions(vector_t *pos, Object objects[], uint8_t numOfObj){
		for(uint8_t i = 0; i < numOfObj; i++){
			if(circleCollision(pos, &(objects[i].pos), &(objects[i].radius))){
				return 1;
			}
		}
		return 0;
	}

	void updatePosition(Body *body, Object objects[], uint8_t numOfObj){
		vector_t newPos = addVectors(&(body ->vel), &(body->pos));
		clampVector(&newPos, 1 << FIX, gameSize); //Keep body within bounds

		if(checkCollisions(&newPos, objects, numOfObj)){
			body -> vel.x = 0;
			body -> vel.y = 0;
		}
		body -> pos = newPos;
	}

	//Aplies linear gravity (F=G*M/r) from all objects to body
	void applyGravity(Body *body, Object objects[], uint8_t numOfObj){
		// Iterate through each object
		for (uint8_t i = 0; i < numOfObj; ++i){
			//Squared Distance from body to object
			uint32_t distanceSquared = distFIXSquared(&body->pos, &objects[i].pos, 10) + 0x400;
			if(distanceSquared < 0x3200 * 0x3200){
				int32_t gravForceMagnitude = (objects[i].mass / distanceSquared);

				// Vector from body to the object. Gives the direction of gravity
				vector_t forceVector = subtractVectors(&(objects[i].pos), &(body -> pos));

				//Now forceVector becomes the actual force applied on the body.
				forceVector.x *= gravForceMagnitude;
				forceVector.y *= gravForceMagnitude;

				forceVector.x = forceVector.x >> 25;
				forceVector.y = forceVector.y >> 25;

	//			//decreasing force. No partiucalr reason for 18. Just seems to fit.
	//			forceVector.x = force.x >> 17;
	//			forceVector.y = force.y >> 17;

				// Update body's velocity
				body->vel.x += forceVector.x;
				body->vel.y += forceVector.y;
			}
		}
	}
	initTimer();

	while(1){
		if(frame){
			frame = 0;
			uint32_t accNum = 0x00000010;
			uint8_t joy1In = readJoy();

			switch (joy1In){
				case joy1UP:
					ship.vel.y -= accNum;
					break;
				case joy1DOWN:
					ship.vel.y += accNum;
					break;
				case joy1LEFT:
					ship.vel.x -= accNum;
					break;
				case joy1RIGHT:
					ship.vel.x += accNum;
					break;
			}
			applyGravity(&ship, objects, numOfObj);

			gotoxy(ship.pos.x >> FIX, ship.pos.y >> FIX);
			printf(" ");

			updatePosition(&ship, objects, numOfObj);

			gotoxy(ship.pos.x >> FIX, ship.pos.y >> FIX);
			printf("o");
		}
	}
}
