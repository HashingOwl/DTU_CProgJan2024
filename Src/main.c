#include <joystick.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include "math.h"
#include "kinematics.h"

#include "soundLib.h"

#include "graphics.h"
#include "GraphicData.h"
#include "main.h"

//Set true by interrupt when it is time to make a new frame
volatile uint8_t updateFrame = 0;

//Set to number > 0 corresponding to the number of frames the collision animation and effect takes. Decreased at every frame if > 0.
int8_t collisionCounter = 0;

int main(void)
{
	uint32_t frameCount = 0;

	//Ship and gravityobjects in game
	GravityTarget ship = {{90 << FIX, 90<<FIX}, {4<<(FIX-2), 1<<(FIX-3)}};
	uint8_t numOfSources = 2;
	GravitySource sources[2] = {{.pos = {20 << FIX, 20 << FIX}, .squareRadius = 5 << FIX, .mass = 0x50000000}, {.pos = {40 << FIX, 60 << FIX}, .squareRadius = 5 << FIX, .mass = 0x50000000}};
	uint8_t numOfBullets = 20;
	GravityTarget bullets[20] = {};

	//Powerups
//	powerup powerups[4] = {};
//	uint8_t numOfPowerups = 4;
//	uint8_t nextPowerupNum = 0;
//	int16_t newPowerupCountdown;

	//Console and graphic
	//int32_t gameSize = consoleSize << FIX;
	uint8_t backgroundContamination[WIDTH * HEIGHT / 8];

	uart_init(2000000);
	initJoystickAnalog();
	//soundInit();
	initTimer15(19, 3600000);
	//newPowerupCountdown = getPowerupCountdown();

	//Drawing game
	drawBackground(BG_Stratosphere_1);
	drawSprite(BG_Stratosphere_1, Alien1_1, 3, 4, RED, sources[0].pos.x >> FIX, sources[0].pos.y >> FIX);
	drawSprite(BG_Stratosphere_1, Alien1_1, 3, 4, RED, sources[1].pos.x >> FIX, sources[1].pos.y >> FIX);

	while(1){
		if(updateFrame){
			updateFrame = 0;
			//newPowerupCountdown--;

			//Getting input from joystick. Passed as reference
			//vector_t input;
			//readJoystickAnalog(&input.x, &input.y);

			//Update velocity based on input
//			ship.vel.x += input.x;
//			ship.vel.y += input.y;

			applyGravity(&ship, sources, numOfSources);
			shipUpdatePosition(&ship, sources, numOfSources);
			//bulletUpdatePosition(bullets, numOfBullets);

			//printVector(&ship.pos);

			//Draw new graphic. Console coordinates are (ship.pos.x >> FIX, ship.pos.y >> FIX)
			drawBullet(ship.pos.x >> FIX, ship.pos.y >> FIX, frameCount, BG_Stratosphere_1);
			cleanRect(backgroundContamination, ship.pos.x >> FIX, ship.pos.y >> FIX, 4, 4);
			drawCleanBackground(BG_Stratosphere_1, backgroundContamination);
			resetGrid(backgroundContamination);
			contaminateRect(backgroundContamination, ship.pos.x >> FIX, ship.pos.y >> FIX, 4, 4);

//			if(newPowerupCountdown == 0){
//
//			}
//
//			for(uint8_t p = 0; p < numOfPowerups; p++){
//
//			}
			frameCount++;
		}
	}
}


//DISSE SKAL RYKKES TIL EN ANDEN FIL
void TIM1_BRK_TIM15_IRQHandler(void) {
	updateFrame = 1;
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

void shipUpdatePosition(GravityTarget *target, GravitySource sources[], uint8_t numOfSources){
	vector_t newPos = addVectors(&(target->vel), &(target->pos));
	clampVector(&newPos, 0, WIDTH << FIX - 1); //Keep body within bounds

	if(checkCollisions(&newPos, sources, numOfSources)){
		target -> vel.x = 0;
		target -> vel.y = 0;
		//MORE CODE TO HANDLE COLLISION
		//Eg. loose af life. Decrease score. Play animation
	}
	target -> pos = newPos;
}

inline int16_t getPowerupCountdown(){
	return 200 + random8bit();
}

void drawBullet(int x, int y, uint32_t frameCount, const uint8_t* background) {
	drawSprite(background, Bullet_Anim[frameCount/8 % 3], 1, 2, WHITE, x, y);
}

void drawPlayer(int x, int y, int alienNum, uint32_t frameCount, const uint8_t* background) {
	switch(alienNum) {
	case 1:
		drawSprite(background, Alien1_Anim[frameCount/8 % 2], 3, 4, GREEN, x, y);
		break;
	case 2:
		drawSprite(background, Alien2_Anim[frameCount/8 % 2], 3, 4, GREEN, x, y);
		break;
	case 3:
		drawSprite(background, Alien3_Anim[frameCount/8 % 2], 3, 4, GREEN, x, y);
		break;
	}
}
//void generateNewPowerup(powerup *powerup, uint8_t nextPowerupNum){
//	powerup newPowerup;
//	powerup[nextPowerupNum]
//}
