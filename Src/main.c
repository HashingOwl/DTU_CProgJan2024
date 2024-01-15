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
	GravityTarget ship = {{2 << FIX, 2<<FIX}, {8<<FIX-2, 1<<FIX-3}};
	uint8_t numOfSources = 2;
	GravitySource sources[2] = {{.pos = {20 << FIX, 20 << FIX}, .squareRadius = 5 << FIX, .mass = 0x50000000}, {.pos = {40 << FIX, 60 << FIX}, .squareRadius = 5 << FIX, .mass = 0x50000000}};
	uint8_t numOfBullets = 20;
	GravityTarget bullets[20] = {};

	//Console
	int16_t consoleSize = 90;
	int32_t gameSize = consoleSize << FIX;

	uart_init(1000000);
	initJoystickAnalog();
	//soundInit();
	initTimer15(0, 3600000);

	printf("\033[2J\033[H");
	gotoxy(sources[0].pos.x >> FIX, sources[0].pos.y >> FIX + 1);
	printf("O");
	gotoxy(sources[1].pos.x >> FIX, sources[1].pos.y >> FIX + 1);
	printf("O");

	void shipUpdatePosition(GravityTarget *target, GravitySource sources[], uint8_t numOfSources){
		vector_t newPos = addVectors(&(target->vel), &(target->pos));
		clampVector(&newPos, 1 << FIX, gameSize); //Keep body within bounds

		if(checkCollisions(&newPos, sources, numOfSources)){
			target -> vel.x = 0;
			target -> vel.y = 0;
			//MORE CODE TO HANDLE COLLISION
				//Eg. loose af life. Decrease score. Play animation
		}
		target -> pos = newPos;
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

			applyGravity(&ship, sources, numOfSources);
			shipUpdatePosition(&ship, sources, numOfSources);
			bulletUpdatePosition(bullets, numOfBullets);

			//Draw new graphic. Console coordinates are (ship.x >> FIX, ship.y >> FIX)
			gotoxy(ship.pos.x >> FIX, ship.pos.y >> FIX + 1);
			printf("o");
		}
	}
}


//DISSE SKAL RYKKES TIL EN ANDEN FIL
void TIM1_BRK_TIM15_IRQHandler(void) {
	frame = 1;
	TIM15->SR &= ~0x0001; // Clear interrupt bit
}

void initTimer15(uint16_t prescale, uint32_t reloadValue){
	RCC->APB2ENR |= RCC_APB2Periph_TIM15;
	TIM15->CR1 &= 0b1111010001110000;
	TIM15->CR1 |= TIM_CR1_CEN;
	TIM15->ARR = reloadValue;
	TIM15->PSC = prescale;

	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts
	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 1); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
}

void bulletUpdatePosition(GravityTarget bullets[], uint8_t numOfBullets){
	for(uint8_t i = 0; i < numOfBullets; i++){
		if(bullets[i].isActive){
			bullets[i].pos.x += bullets[i].vel.x;
			bullets[i].pos.y += bullets[i].vel.y;
		}
	}
}
