#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "math.h"
#include "kinematics.h"
#include "io.h"
#define FIXMUL(a, b) ((a>>8)*(b>>8))
#define BIGFIXMUL (a, b) (((a>>12)*(b>>12)) << 8)

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
	TIM15->ARR = (uint8_t) 10;
	TIM15->PSC = 0x1F40;

	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts
	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 1); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
}

int main(void)
{
	Body ship = {{2 << 16, 2<<16}, {}};
	Object objects[2] = {{.pos = {20 << 16, 20 << 16}, .radius = 1 << 16, .mass = 0x50000000}, {.pos = {40 << 16, 60 << 16}, .radius = 1 << 16, .mass = 0x50000000}};
	uint8_t numOfObj = 2;
	int16_t consoleSize = 90;
	int32_t gameSize = consoleSize << 16;
	uart_init(1000000);
	printf("\033[2J\033[H");
	setupJoystickPins();
	gotoxy(objects[0].pos.x >> 16, objects[0].pos.y >> 16);
	printf("O");
	gotoxy(objects[1].pos.x >> 16, objects[1].pos.y >> 16);
	printf("O");

	char circleCollision(vector_t *p, vector_t *q, uint32_t *radius){
		return distFIXSquared(p, q) < *radius;
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
		vector_t change = multFIXVector(&(body ->vel), 0x00000100);
		vector_t newPos = addVectors(&change, &(body->pos));
		clampVector(&newPos, 1 << 16, gameSize); //Keep body within bounds

		if(checkCollisions(&newPos, objects, numOfObj)){
			body -> vel.x = 0;
			body -> vel.y = 0;
		}
		body -> pos = newPos;
	}
	void applyGravity(Body *body, Object objects[], uint8_t numOfObj){
		uint32_t offset = 2 << 16 * 0;
		// Iterate through each object
		for (uint8_t i = 0; i < numOfObj; ++i){

			//Squared Distance from body to object + offset for smoothness
			uint32_t distanceSquared = distFIXSquared(&body->pos, &objects[i].pos) + offset;
			// Vector from body to the object
			vector_t displacement = subtractVectors(&(objects[i].pos), &(body -> pos));
			int32_t gravForceMagnitude = objects[i].mass / (distanceSquared >> 16); //Bitshiftning so distanceSquared is way smaller than massG, thus getting higher precision
			vector_t force = multFIXVector(&displacement, gravForceMagnitude);

			//decreasing force. No partiucalr reason for 18. Just seems to fit.
			force.x = force.x >> 17;
			force.y = force.y >> 17;

			// Update body's velocity
			body->vel = addVectors(&body->vel, &force);
		}
	}
	initTimer();

	while(1){
		if(frame){
			frame = 0;
			uint32_t accNum = 0x00000400;
			vector_t acc = {};
			uint8_t joy1In = readJoy();

			switch (joy1In){
			case joy1UP:
				acc.y = -accNum;
				break;
			case joy1DOWN:
				acc.y = accNum;
				break;
			case joy1LEFT:
				acc.x = -accNum;
				break;
			case joy1RIGHT:
				acc.x = accNum;
				break;
			case joy1CENTER:
				ship.vel.x = 0;
				ship.vel.y = 0;
			}
			ship.vel = addVectors(&acc, &ship.vel);
			applyGravity(&ship, objects, numOfObj);

			gotoxy(ship.pos.x >> 16, ship.pos.y >> 16);
			printf(" ");

			updatePosition(&ship, objects, numOfObj);

			gotoxy(ship.pos.x >> 16, ship.pos.y >> 16);
			printf("o");
		}
	}
}
