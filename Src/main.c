#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "math.h"
#include "kinematics.h"
#include "io.h"
#include "Joystick.h"
#include "main.h"

//Set true by interrupt when it is time to make a new frame
volatile uint8_t frame = 0;

//SKAL SLETTES
void gotoxy(int x, int y){
	printf("\033[%d;%dH", y, x);
}

int main(void)
{
	//Ship and gravityobjects in game
	Body ship = {{2 << FIX, 2<<FIX}, {8<<FIX-2, 1<<FIX-3}};
	uint8_t numOfObj = 2;
	Object objects[2] = {{.pos = {20 << FIX, 20 << FIX}, .radius = 5 << FIX, .mass = 0x50000000}, {.pos = {40 << FIX, 60 << FIX}, .radius = 5 << FIX, .mass = 0x50000000}};
	uint8_t numOfBullets = 20;
	Body bullets[20] = {};

	//Console
	int16_t consoleSize = 90;
	int32_t gameSize = consoleSize << FIX;

	uart_init(1000000);
	initJoystickAnalog();
	soundInit();
	initTimer();

	printf("\033[2J\033[H");
	gotoxy(objects[0].pos.x >> FIX, objects[0].pos.y >> FIX + 1);
	printf("O");
	gotoxy(objects[1].pos.x >> FIX, objects[1].pos.y >> FIX + 1);
	printf("O");

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

			//Getting input from joystick. Passed as reference
			vector_t input;
			readJoystickAnalog(&input.x, &input.y);

			//Update velocity based on input
			ship.vel.x += input.x;
			ship.vel.y += input.y;

			//Remove old graphic. Console coordinates are (ship.x >> FIX, ship.y >> FIX)
			gotoxy(ship.pos.x >> FIX, ship.pos.y >> FIX + 1);
			printf(" ");

			applyGravity(&ship, objects, numOfObj);
			shipUpdatePosition(&ship, objects, numOfObj);
			bulletUpdatePosition(bullets, numOfBullets);

			//Draw new graphic. Console coordinates are (ship.x >> FIX, ship.y >> FIX)
			gotoxy(ship.pos.x >> FIX, ship.pos.y >> FIX + 1);
			printf("o");
		}
	}
}


//DISSE SKAL RYKKES TIL EN ANDEN FIL
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

void bulletUpdatePosition(Body bullets[], uint8_t numOfBullets){
	for(uint8_t i = 0; i < numOfBullets; i++){
		if(bullets[i].draw){
			bullets[i].pos.x += bullets[i].vel.x;
			bullets[i].pos.y += bullets[i].vel.y;
		}
	}
}
