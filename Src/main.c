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
	// Init modules
	uart_init(2000000);
	initJoystickAnalog();
	soundInit();
	disableMusic();
	initTimer15(19, 3600000);

	// While using mini joystick as substitute for proper joystick
	setupJoystickPins();

	int gameState;
	gameState = PLAYING;
	// Used in sprite animations
	uint32_t frameCount = 0;

	//Ship and gravity objects in game
	GravityTarget ship = {
			.pos = {64 << FIX, 64<<FIX},
			.vel = {0, 0},
			.anchor = {6, 4},
			.squareRadius = 4*4 << FIX};

	// Asteroids creating gravity
	uint8_t numAsteroids = 2;
	GravitySource asteroids[2] = {
			{.pos = {36 << FIX, 36 << FIX}, .anchor = {8, 8}, .squareRadius = 8*8 << FIX, .mass = 0x50000000},
			{.pos = {92 << FIX, 92 << FIX}, .anchor = {8, 8}, .squareRadius = 8*8 << FIX, .mass = 0x50000000}};
	uint8_t numBullets = 20;
	GravityTarget bullets[20] = {};

	//Console and graphic
	//int32_t gameSize = consoleSize << FIX;
	uint8_t backgroundContamination[WIDTH * HEIGHT / 8] = {};
	// Not actually a constant, it can be changed in software. It's a pointer to a constant.
	const uint8_t* currentBackground = BG_Stratosphere_1;

	//Miscenlaneous
	vector_t asteroidPrintOffset = {-0x500, -0x400};
	vector_t shipPrintOffset = {-0x100, -0x100};

	// INITIALISATION DRAWING: Shoudl be made whenever we set the gamemode to PLAYING
	drawBackground(currentBackground);
	// Asteroids
	for (int i = 0; i < numAsteroids; i++) {
		drawAsteroid(&asteroids[i], currentBackground);
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
				vector_t input = {0, 0};
				//readJoystickAnalog(&input.x, &input.y);
				// Temporary when testing from a board without proper joystick. Please outcomment it instead of deleting.
				uint8_t joyVal = readJoy();
				input.y -= ((joyVal & 1) != 0);
				input.y += ((joyVal & 2) != 0);
				input.x -= ((joyVal & 4) != 0);
				input.x += ((joyVal & 8) != 0);

				//Update velocity based on input
				ship.vel.x += input.x * 16;
				ship.vel.y += input.y * 16;

				applyGravity(&ship, asteroids, numAsteroids);
				shipUpdatePosition(&ship, asteroids, numAsteroids);
				bulletUpdatePosition(bullets, numBullets);

				//-------------------Drawing--------------------------------
				// Player
				drawAlien(&ship, 1, frameCount, currentBackground);
				cleanRect(backgroundContamination, (ship.pos.x >> FIX) - ship.anchor.x, (ship.pos.y >> FIX) - ship.anchor.y, 12, 8);

				// Clean Background
				drawCleanBackground(currentBackground, backgroundContamination);
				resetGrid(backgroundContamination);

				//------------Contaminate-for-next-frame--------------------
				// Player
				contaminateRect(backgroundContamination, (ship.pos.x >> FIX) - ship.anchor.x, (ship.pos.y >> FIX) - ship.anchor.y, 12, 8);

//				if(newPowerupCountdown == 0){
//
//				}
//
//				for(uint8_t p = 0; p < numOfPowerups; p++){
//
//				}
				break;
//=========================================PAUSED======================================================
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

void drawBullet(GravityTarget* bullet, uint32_t frameCount, const uint8_t* background) {
	drawSprite(background, Bullet_Anim[frameCount/8 % 3], 1, 2, WHITE, (bullet->pos.x >> FIX) - bullet->anchor.x, (bullet->pos.y >> FIX) - bullet->anchor.y);
}

void drawAsteroid(GravitySource* asteroid, const uint8_t* background) {
	drawSprite(background, Asteroid_1, 4, 8, YELLOW, (asteroid->pos.x >> FIX) - asteroid->anchor.x, (asteroid->pos.y >> FIX) - asteroid->anchor.y);
}

void drawAlien(GravityTarget* alien, int alienNum, uint32_t frameCount, const uint8_t* background) {
	switch(alienNum) {
	case 1:
		drawSprite(background, Alien1_Anim[frameCount/8 % 2], 3, 4, GREEN, (alien->pos.x >> FIX) - alien->anchor.x, (alien->pos.y >> FIX) - alien->anchor.y);
		break;
	case 2:
		drawSprite(background, Alien2_Anim[frameCount/8 % 2], 3, 4, GREEN, (alien->pos.x >> FIX) - alien->anchor.x, (alien->pos.y >> FIX) - alien->anchor.y);
		break;
	case 3:
		drawSprite(background, Alien3_Anim[frameCount/8 % 2], 3, 4, GREEN, (alien->pos.x >> FIX) - alien->anchor.x, (alien->pos.y >> FIX) - alien->anchor.y);
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
