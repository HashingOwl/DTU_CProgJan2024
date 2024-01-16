#include <joystick.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include "math.h"
#include "kinematics.h"
#include "io.h"
#include "soundLib.h"

#include "graphics.h"
#include "GraphicData.h"
#include "main.h"

// Game states
#define MENU 1
#define PLAYING 2
#define PAUSED 3

#define MIN_X 0x200 //2.00
#define MAX_X 0x7B00 //123.00
#define MIN_Y 0x200 //2.00
#define MAX_Y 0x7B00 //123.00

//Set true by interrupt when it is time to make a new frame
volatile uint8_t updateFrame = 0;

//Set to number > 0 corresponding to the number of frames the collision animation and effect takes. Decreased at every frame if > 0.
int8_t collisionCounter = 0;

int main(void)
{
	int gameState;
	gameState = PLAYING;
	// Used in sprite animations
	uint32_t frameCount = 0;

	//Ship and gravity objects in game
	GravityTarget ship = {{100 << FIX, 5<<FIX}, {0x50, 0x50}};
	uint8_t numOfSources = 2;
	GravitySource sources[2] = {
			{.pos = {40 << FIX, 50 << FIX}, .squareRadius = 0x3100, .mass = 0x50000000},
			{.pos = {90 << FIX, 90 << FIX}, .squareRadius = 0x3100, .mass = 0x50000000}};
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
	uint8_t *currentBackground = BG_Stratosphere_1;

	//Miscenlaneous
	vector_t asteroidPrintOffset = {-0x500, -0x400};
	vector_t shipPrintOffset = {-0x100, -0x100};

	uart_init(1000000);
	initJoystickAnalog();
	soundInit();
	disableMusic();
	initTimer15(19, 3600000);
	//newPowerupCountdown = getPowerupCountdown();

	// While using mine joystick as substitute for proper joystick
	setupJoystickPins();

	//Drawing game
	drawBackground(currentBackground);
	for(uint8_t i = 0; i < numOfSources; i++){
		drawSprite(TestBG, Alien1_1, 3, 4, RED, (sources[i].pos.x + asteroidPrintOffset.x) >> FIX, (sources[i].pos.y + asteroidPrintOffset.y) >> FIX);
	}

	while(1){
		if(updateFrame){
			updateFrame = 0;
			frameCount++;

			switch (gameState) {
//==========================================MENU========================================================
			case MENU:
				break;
//=========================================PLAYING======================================================
			case PLAYING:;
				//Getting input from joystick. Passed as reference
				vector_t input;
				readJoystickAnalog(&input.x, &input.y);

				//Update velocity based on input
				ship.vel.x += input.x >> 2;
				ship.vel.y += input.y >> 3;

				applyGravity(&ship, sources, numOfSources);
				shipUpdatePosition(&ship, sources, numOfSources);
				bulletUpdatePosition(bullets, numOfBullets);

				//DEBUG
//				bgcolor(BLACK);
//				fgcolor(WHITE);
//				gotoxy(0,0);
//				printVector(&ship.pos);
//				gotoxy(0,1);
//				printVector(&ship.vel);

				vector_t shipPrintPos = {(ship.pos.x + shipPrintOffset.x) >> FIX, (ship.pos.y + shipPrintOffset.y) >> FIX};

				drawSprite(currentBackground, Bullet_1, 1, 2, GREEN, shipPrintPos.x, shipPrintPos.y);
				cleanRect(backgroundContamination, shipPrintPos.x, shipPrintPos.y, 4, 4);
				drawCleanBackground(currentBackground, backgroundContamination);
				resetGrid(backgroundContamination);
				contaminateRect(backgroundContamination, shipPrintPos.x, shipPrintPos.y, 4, 4);

//				if(newPowerupCountdown == 0){
//
//				}
//
//				for(uint8_t p = 0; p < numOfPowerups; p++){
//
//				}
				break;
//=========================================PLAYING======================================================
			case PAUSED:
				// Check for input, showing either boss_bakcground, or changing state to MENU or PLAYING
				break;
			}
		}
	}
}

void shipUpdatePosition(GravityTarget *target, GravitySource sources[], uint8_t numOfSources){
	vector_t newPos = addVectors(&(target->vel), &(target->pos));


	if(outOfBounds(newPos.x, MIN_X, MAX_X))
		target->vel.x = 0;
	if(outOfBounds(newPos.y, MIN_Y, MAX_Y))
		target->vel.y = 0;

	clampVector(&newPos, MIN_X, MAX_X); //Keep body within bounds

	if(checkCollisions(&newPos, sources, numOfSources)){
		target -> vel.x = 0;
		target -> vel.y = 0;
		//MORE CODE TO HANDLE COLLISION
			//Eg. loose af life. Decrease score. Play animation
	}

	target -> pos = newPos;
}

void bulletUpdatePosition(GravityTarget bullets[], uint8_t numOfBullets){
	for(uint8_t i = 0; i < numOfBullets; i++){
		if(bullets[i].isActive){
			bullets[i].pos.x += bullets[i].vel.x;
			bullets[i].pos.y += bullets[i].vel.y;
		}
	}
}

//inline int16_t getPowerupCountdown(){
//	return 200 + random8bit();
//}
//void generateNewPowerup(powerup *powerup, uint8_t nextPowerupNum){
//	powerup newPowerup;
//	powerup[nextPowerupNum]
//}

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
