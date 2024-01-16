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

// Fixed point constants
#define BORDER_PAD 	(2 << FIX)
#define U_WIDTH 	(WIDTH << FIX)
#define U_HEIGHT 	(HEIGHT * 2 << FIX)
/*
#define MIN_X 0x200 //2.00
#define MAX_X 0x7B00 //123.00
#define MIN_Y 0x200 //2.00
#define MAX_Y 0x7B00 //123.00
*/

//Set true by interrupt when it is time to make a new frame
volatile uint8_t updateFrame = 0;

//Set to number > 0 corresponding to the number of frames the collision animation and effect takes. Decreased at every frame if > 0.
int8_t collisionCounter = 0;

// Used for debugging
long int debug1 = 0;
long int debug2 = 0;

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
			.radius = 5 << FIX};

	// Asteroids creating gravity
	uint8_t numAsteroids = 2;
	GravitySource asteroids[2] = {
			{.pos = {36 << FIX, 36 << FIX}, .anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 12},
			{.pos = {92 << FIX, 92 << FIX}, .anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 12}};
	uint8_t numBullets = 20;
	GravityTarget bullets[20] = {};

	//Console and graphic
	//int32_t gameSize = consoleSize << FIX;
	uint8_t backgroundContamination[WIDTH * HEIGHT / 8] = {};
	// Not actually a constant, it can be changed in software. It's a pointer to a constant.
	const uint8_t* currentBackground = BG_Stratosphere_1;

	// INITIALISATION DRAWING: Shoudl be made whenever we set the gamemode to PLAYING
	drawBackground(currentBackground);
	// Asteroids
	for (int i = 0; i < numAsteroids; i++) {
		drawAsteroid(&asteroids[i], currentBackground);
	}

	vector_t debugVec = {2 << FIX, 4 << FIX};
	debugVec = normalizeFIXVector(&debugVec);
	debug1 = debugVec.x;
	debug2 = debugVec.y;

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
				//-------------------INPUT----------------------------------
				//Getting input from joystick. Passed as reference
				vector_t input = {0, 0};
				//readJoystickAnalog(&input.x, &input.y);
				// Temporary when testing from a board without proper joystick. Please outcomment it instead of deleting.
				uint8_t joyVal = readJoy();
				input.y -= ((joyVal & 1) != 0) * 20;
				input.y += ((joyVal & 2) != 0) * 20;
				input.x -= ((joyVal & 4) != 0) * 20;
				input.x += ((joyVal & 8) != 0) * 20;

				//------------------PHYSICS---------------------------------

				//Update velocity based on input
				ship.vel.x += input.x;
				ship.vel.y += input.y;

				applyGravity(&ship, asteroids, numAsteroids);

				shipUpdatePosition(&ship);
				bulletUpdatePosition(bullets, numBullets);

				// Player-Bounds collision
				if(outOfBounds(ship.pos.x, 	ship.radius + BORDER_PAD, U_WIDTH - ship.radius - BORDER_PAD)) {
					clamp(&(ship.pos.x), 	ship.radius + BORDER_PAD, U_WIDTH - ship.radius - BORDER_PAD);
					ship.vel.x /= -2;
				}
				if(outOfBounds(ship.pos.y, 	ship.radius + BORDER_PAD, U_HEIGHT - ship.radius - BORDER_PAD)) {
					clamp(&(ship.pos.y), 	ship.radius + BORDER_PAD, U_HEIGHT - ship.radius - BORDER_PAD);
					ship.vel.y /= -2;
				}

				// Player-Asteroids collision
				for (int i = 0; i < numAsteroids; i++) {
					// Square of
					if (circleCollision(&(ship.pos), &(asteroids[i].pos), FIXSQUARE(ship.radius + asteroids[i].radius))) {
						// Do a spherical bounce of velocity, and move out player a bit.
						vector_t normal = subtractVectors(&ship.pos, &asteroids[i].pos);
						normal = normalizeFIXVector(&normal);
						vector_t impulse = multFIXVector(&normal, -2*dotFIX(&ship.vel, &normal));
						ship.vel = addVectors(&ship.vel, &impulse);
						// Ships speed is halved after reflection
						ship.vel = multFIXVector(&ship.vel, (1 << FIX) >> 1);
						// Nudge the ship out as well. Not an ideal solution, but it's sufficient
						vector_t nudge = multFIXVector(&normal, 2 << FIX);
						ship.pos = addVectors(&ship.pos, &nudge);
					}
				}

				//-------------------Drawing--------------------------------
				// Player
				drawAlien(&ship, 1, frameCount, currentBackground);
				cleanRect(backgroundContamination, (ship.pos.x >> FIX) - ship.anchor.x, (ship.pos.y >> FIX) - ship.anchor.y, 12, 8);

				// Clean Background
				drawCleanBackground(currentBackground, backgroundContamination);
				resetGrid(backgroundContamination);

				gotoxy(0,0); printf("%4ld", debug1);
				gotoxy(0,1); printf("%4ld", debug2);

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

void shipUpdatePosition(GravityTarget *ship){
	ship->pos = addVectors(&(ship->pos), &(ship->vel));
}

// Things removed from this function
/*
 	 vector_t newPos = addVectors(&(target->vel), &(target->pos));
	target -> pos = newPos;

	clamp(&(newPos.x), target->radius + BORDER_PAD, U_WIDTH - BORDER_PAD); //Keep body within bounds
	clamp(&(newPos.y), target->radius + BORDER_PAD, U_HEIGHT - BORDER_PAD); //Keep body within bounds

 	if(outOfBounds(newPos.x, MIN_X, MAX_X))
		target->vel.x = 0;
	if(outOfBounds(newPos.y, MIN_Y, MAX_Y))
		target->vel.y = 0;

	if(checkCollisions(&newPos, sources, numOfSources)){
		target -> vel.x = 0;
		target -> vel.y = 0;
		//MORE CODE TO HANDLE COLLISION
		//Eg. loose af life. Decrease score. Play animation
	}
 */



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

// Drawing functions
#define FIX_2_X(a) ((a->pos.x >> FIX) - a->anchor.x)
#define FIX_2_Y(a) ((a->pos.y >> FIX) - a->anchor.y)

void drawBullet(GravityTarget* bullet, uint32_t frameCount, const uint8_t* background) {
	drawSprite(background, Bullet_Anim[frameCount/8 % 3], 1, 2, WHITE, FIX_2_X(bullet), FIX_2_Y(bullet));
}

void drawAsteroid(GravitySource* asteroid, const uint8_t* background) {
	drawSprite(background, Asteroid_1, 5, 10, BROWN, FIX_2_X(asteroid), FIX_2_Y(asteroid));
}

void drawAlien(GravityTarget* alien, int alienNum, uint32_t frameCount, const uint8_t* background) {
	switch(alienNum) {
	case 1:
		drawSprite(background, Alien1_Anim[frameCount/8 % 2], 3, 4, GREEN, FIX_2_X(alien), FIX_2_Y(alien));
		break;
	case 2:
		drawSprite(background, Alien2_Anim[frameCount/8 % 2], 3, 4, GREEN, FIX_2_X(alien), FIX_2_Y(alien));
		break;
	case 3:
		drawSprite(background, Alien3_Anim[frameCount/8 % 2], 3, 4, GREEN, FIX_2_X(alien), FIX_2_Y(alien));
		break;
	}
}

void drawSentry() {

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
