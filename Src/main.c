#include <io.h>
#include "joystickAnalog.h"
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include "math.h"
#include "kinematics.h"
#include "soundLib.h"

#include "graphics.h"
#include "GraphicData.h"
#include "main.h"
#include "Highscore.h"
// Game states
#define MENU 1
#define PLAYING 2
#define HELP 3

// Fixed point constants
#define BORDER_PAD 	(2 << FIX)
#define U_WIDTH 	(P_WIDTH << FIX)
#define U_HEIGHT 	(P_HEIGHT * 2 << FIX)

#define FIX_2_X(a) ((a->pos.x >> FIX) - a->anchor.x)
#define FIX_2_Y(a) ((a->pos.y >> FIX) - a->anchor.y)

#define speedPU 0
#define gravityPU 1
/*
#define MIN_X 0x200 //2.00
#define MAX_X 0x7B00 //123.00
#define MIN_Y 0x200 //2.00
#define MAX_Y 0x7B00 //123.00
*/

//Set true by interrupt when it is time to make a new frame
volatile uint8_t updateFrame = 0;

// Used for debugging
long int debug1 = 0;
long int debug2 = 0;

int main(void)
{
	//----------GAME CONTROL---------
	uint32_t gameState = PLAYING;
	uint32_t frameCount = 0; // Used in sprite animations

	uint8_t aliensLeftStart = 3; //Max number of aliens to die before game over
	uint8_t aliensLeft = aliensLeftStart;
	uint8_t aliensThrough = 0; //score

	//----------PLAYER ALIEN---------
	vector_t playerStartPos = {100 << FIX, 10<<FIX};
	GravityTarget ship = {
			.pos = playerStartPos,
			.vel = {0, 0},
			.anchor = {6, 4},
			.radius = 5 << FIX};
	uint8_t playerHit = 0;
	uint8_t maxLives = 2;
	uint8_t livesLeft = maxLives;
	uint8_t currentAlien = 1;


	//----------ASTEROIDS-------------
	uint8_t numAsteroids = 7;
	GravitySource asteroids[7] = {
			{.pos = {U_WIDTH/2, U_HEIGHT/2}, .anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
			{.pos = {40 << FIX, 50 << FIX}, .anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
			{.pos = {40 << FIX, 150 << FIX}, .anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
			{.pos = {160 << FIX, 50 << FIX}, .anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
			{.pos = {160 << FIX, 150 << FIX}, .anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
			{.pos = {100 << FIX, 30 << FIX}, .anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
			{.pos = {100 << FIX, 170 << FIX}, .anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
	};

	//----------ENEMIES--------------
	vector_t enemies[2] = {
			{70 << FIX, 100 << FIX},
			{130 << FIX, 100 << FIX},
	};
	uint8_t numOfEnemies = 2;
	int16_t enemyShootResetValue = 20 * 5; // 20 * seconds between shoot
	int16_t enemyShootCountdown = enemyShootResetValue;

	//----------BULLETS--------------
	uint8_t numBullets = 20;
	bullet bullets[20] = {};
	uint16_t bulletSpeed = 0x180;
	uint16_t bulletSpeedIncrease = 0x50;

	//----------POWERUPS--------------
	char RNGSeedSet = 0;
	uint16_t powerupResetValue = 20*10;
	uint16_t powerupCountdown = 100;
	powerup powerups[4] = {};
	uint8_t numPowerups = 4;
	uint8_t powerupEffects[2] = {};

	//----------MENU------------------
	uint8_t joyVal;
	uint8_t joyValPrev;
	int8_t currSelectionMainMenu = 0;
	uint8_t buttonLift = 1;

	//----------CONSOLE AND GRAPHICS--------------
	uint8_t backgroundContamination[P_WIDTH * P_HEIGHT / 8] = {};
	const uint8_t* currentBackground = BG_Stratosphere_2; // Not actually a constant, it can be changed in software. It's a pointer to a constant.

	//----------INITIALISATION OF HARD- AND SOFTWARE----------
	// Output
	uart_init(1000000);
	initLED();
	setLED(BLUE);
	//initLCD();

	//Input
	initAnalogJoystick();
	initDigitalJoystick(); // While using mini joystick as substitute for proper joystick
	joyVal = readAnalogJoystickDigital();
	joyValPrev = joyVal;

	//Music
	soundInit();
	changeMusic(0);
	initTimer15(19, 3600000);

	//Miscenlaneous
	//initBossScreen();
	resetGrid(backgroundContamination);

	if(gameState == PLAYING)
		initGame(currentBackground, asteroids, numAsteroids, livesLeft);


	//MAIN LOOP
	while(1){
		if(updateFrame){
			updateFrame = 0;
			frameCount++;

			switch (gameState) {
//==========================================MENU========================================================
			case MENU:
				joyVal = readAnalogJoystickDigital();
				if (joyVal != joyValPrev) {
					for(uint8_t i = 0;i < 2; i++){
						contaminateRect(backgroundContamination,46+i*95,90+currSelectionMainMenu*38,12,12);
					}

					if (joyVal & 0b1) {
						currSelectionMainMenu--;
					}
					else if ( (joyVal & 0b10) && joyVal != 0) {
						currSelectionMainMenu++;
					}

				}
				if (currSelectionMainMenu >2) {
					currSelectionMainMenu = 0;
				}
				else if(currSelectionMainMenu < 0) {
					currSelectionMainMenu = 2;
				}
				joyValPrev = joyVal;

				drawCleanBackground(currentBackground, backgroundContamination);
				resetGrid(backgroundContamination);

				for(uint8_t i = 0;i < 2; i++){
					drawSprite(currentBackground, Alien1_Anim[0], 3, 4, WHITE, 46+i*95,90+currSelectionMainMenu*38);
				}
				if (readJoystickButtons() && buttonLift) {
					buttonLift = 0;
					switch(currSelectionMainMenu) {
						//Goto game
						case 0:
							//TODO add reset here.
							currentBackground = BG_Stratosphere_2;
							gameState = PLAYING;
							initGame(currentBackground, asteroids, numAsteroids, livesLeft);
							break;
						//Goto help screen
						case 1:
							currentBackground = BG_Stratosphere_2;
							drawBackground(currentBackground);//TODO tilføj den rigtige skærm for help
							gameState = HELP;
							break;
						//Dis/en-able sound
						case 2:
							changeMusic(0);
							break;

					}
				}
				else if (readJoystickButtons() == 0){
					buttonLift = 1;
				}
				break;
//=========================================PLAYING======================================================
			case PLAYING:
				enemyShootCountdown--;
				powerupCountdown--;

				//If button to enter main menu is hit go to main menu
				if (readJoystickButtons() & 0) {
					currentBackground=MainMenuBG;
					drawBackground(currentBackground);
				}

				if(playerHit > 0)
					playerHit--;

				//-------------------INPUT----------------------------------
				//Getting input from joystick. Passed as reference
				vector_t input = {0, 0};
				//readJoystickAnalog(&input.x, &input.y);
				// Temporary when testing from a board without proper joystick. Please outcomment it instead of deleting.
				joyVal = readAnalogJoystickDigital();
				input.y -= ((joyVal & 1) != 0) * 20;
				input.y += ((joyVal & 2) != 0) * 20;
				input.x -= ((joyVal & 4) != 0) * 20;
				input.x += ((joyVal & 8) != 0) * 20;

				//------------------PLAYER PHYSICS---------------------------------
				//Update velocity based on input
				ship.vel.x += input.x;
				ship.vel.y += input.y;

				applyGravity(ship.pos, &ship.vel, asteroids, numAsteroids);
				shipUpdatePosition(&ship);


				// Player-Bounds collision
				if(outOfBounds(ship.pos.x, 	ship.radius + BORDER_PAD, U_WIDTH - ship.radius - BORDER_PAD)) {
					clamp(&(ship.pos.x), 	ship.radius + BORDER_PAD, U_WIDTH - ship.radius - BORDER_PAD);
					ship.vel.x /= -4;
				}
				//Check if player hits top
				if(ship.pos.y < ship.radius + BORDER_PAD){
					ship.pos.y = ship.radius + BORDER_PAD;
					ship.vel.y /= -4;
				}

				//WIN CONDITION - Check if player hits bottom.
				if(ship.pos.y > U_WIDTH - ship.radius - BORDER_PAD){
					aliensThrough++;
					bulletSpeed += bulletSpeedIncrease;
					livesLeft = maxLives;
					setLEDToIndicateHealth(livesLeft);

					//TODO ALBERT print new score

					makeNewAlien(&ship, &playerStartPos, &currentAlien);
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

				// --------------------------BULLETS AND ENEMIES--------------------------------
				for(uint8_t b = 0; b < numBullets; b++){
					applyGravity(bullets[b].pos, &bullets[b].vel, asteroids, numAsteroids);
				}
				bulletUpdatePosition(bullets, numBullets, asteroids, numAsteroids);
				if(bulletHitPlayer(&ship.pos, bullets, numBullets)){
					livesLeft--;
					playerHit = 25;
					if(livesLeft == 0){
						aliensLeft--;
						livesLeft = maxLives;

						makeNewAlien(&ship, &playerStartPos, &currentAlien);
						if(aliensLeft == 0){
							gameState = MENU;
						}
					}
					setLEDToIndicateHealth(livesLeft);
				}
				drawAllBullets(bullets, numBullets, frameCount, currentBackground);

				// Clean Bullets
				for (int i = 0; i < numBullets; i++) {
					if (bullets[i].isActive) {
						cleanRect(backgroundContamination, (bullets[i].pos.x >> FIX) - bullets[i].anchor.x, (bullets[i].pos.y >> FIX) - bullets[i].anchor.y, 4, 4);
					}
				}

				if(!enemyShootCountdown){
					generateBullets(bullets, numBullets, enemies, numOfEnemies, &ship.pos, bulletSpeed);
					enemyShootCountdown = enemyShootResetValue;
				}

				//--------------------------POWERUPS---------------------------------
				//Generating powerups
				if(powerupCountdown == 0){
					powerupCountdown = powerupResetValue;
					if(!RNGSeedSet){
						RNGSeedSet = 1;
						setRNGSeed((ship.pos.x & 0x000000FF) * (ship.pos.y & 0x000000FF)); //Semi random way to initialize RNG
					}
					generateNewPowerup(powerups, numPowerups);
					powerupCountdown = getPowerupCountdown();
				}

				//Checking for colliossn with powerups
				for(uint8_t p = 0; p < numPowerups; p++){
					if(circleCollision(&powerups[p].pos, &ship.pos, 0xF00)){
						powerups[p].isActive = 0;
						//todo ROALD remove powerup;
						//todo ROALD apply powerup effect;
							//If necessary also make check for end of powerup
					}
				}

				//-------------------Drawing--------------------------------
				// Player
				drawAlien(&ship, currentAlien, frameCount, currentBackground, playerHit);
				cleanRect(backgroundContamination, (ship.pos.x >> FIX) - ship.anchor.x, (ship.pos.y >> FIX) - ship.anchor.y, 12, 8);

				// Clean Background
				drawCleanBackground(currentBackground, backgroundContamination);
				resetGrid(backgroundContamination);

				gotoxy(0,0); printf("%4ld", debug1);
				gotoxy(0,1); printf("%4ld", debug2);

				//------------Contaminate-for-next-frame--------------------
				// Player
				contaminateRect(backgroundContamination, (ship.pos.x >> FIX) - ship.anchor.x, (ship.pos.y >> FIX) - ship.anchor.y, 12, 8);
				// Bullet
				for (int i = 0; i < numBullets; i++) {
					if (bullets[i].isActive) {
						contaminateRect(backgroundContamination, (bullets[i].pos.x >> FIX) - bullets[i].anchor.x, (bullets[i].pos.y >> FIX) - bullets[i].anchor.y, 4, 4);
					}
				}

				break;
//=========================================HELP======================================================
			case HELP:
				// Check for input, showing either boss_bakcground, or changing state to MENU or PLAYING
				if (readJoystickButtons() && buttonLift) {
					//TODO tilføj at gemme score.
					buttonLift = 0;
					currentBackground = MainMenuBG;
					drawBackground(currentBackground);
					gameState = MENU;
				}
				else if (readJoystickButtons() == 0){
					buttonLift = 1;
				};
			}
		}
	}
}

//----------------------------------GAME CONTROL----------------------------
void initGame(uint8_t* currentBackground, GravitySource asteroids[], uint8_t numAsteroids, uint8_t livesLeft){
	drawBackground(currentBackground);
	setLEDToIndicateHealth(livesLeft);

	//Draw asteroids
	for (int i = 0; i < numAsteroids; i++) {
		drawAsteroid(&asteroids[i], currentBackground);
		// Delay - without this it doesn't work for mysterious reasons.
		for (uint32_t i = 0; i < 360000; i++);
	}
}

void shipUpdatePosition(GravityTarget *ship){
	ship->pos = addVectors(&(ship->pos), &(ship->vel));
}
void makeNewAlien(GravityTarget *ship, vector_t *startPos, uint8_t* currentAlien){
	ship->pos = *startPos;

	ship->vel.y = 0;
	ship->vel.y = 0;

	(*currentAlien)++;
	if(*currentAlien == 4)
		*currentAlien = 1;
}

//-----------------------------------BULLETS------------------------------------------------------
void bulletUpdatePosition(bullet bullets[], uint8_t numOfBullets, GravitySource asteroids[], uint8_t numAsteroids){
	for(uint8_t i = 0; i < numOfBullets; i++){
		if(bullets[i].isActive){
			bullets[i].pos.x += bullets[i].vel.x;
			bullets[i].pos.y += bullets[i].vel.y;

			//Asteroid collision
			for(uint8_t a = 0; a < numAsteroids; a++){
				if(circleCollision(&bullets[i].pos, &asteroids[a].pos, FIXSQUARE(asteroids[a].radius + bullets[i].radius))){
					bullets[i].isActive = 0;
					break;
				}
			}

			//Out of bounds
			if ( outOfBounds(bullets[i].pos.x, BORDER_PAD, U_WIDTH - BORDER_PAD) ) {
				bullets[i].isActive = 0;
			}
			else if ( outOfBounds(bullets[i].pos.y, BORDER_PAD, U_HEIGHT - BORDER_PAD) ) {
				bullets[i].isActive = 0;
			}
		}
	}
}

char bulletHitPlayer(vector_t* playerPos, bullet bullets[], uint8_t numBullets){
	char hit = 0;
	for (uint8_t i = 0; i < numBullets; i++)
	{
		if(bullets[i].isActive){
			if(circleCollision(playerPos, &bullets[i].pos, 0x1900)){
				hit = 1;
				bullets[i].isActive = 0;
			}
		}
	}
	return hit;
}

void drawAllBullets(bullet bullets[], uint8_t numOfBullets, uint32_t frameCount, const uint8_t* background){
	for(uint8_t i = 0; i < numOfBullets; i++){
		if(bullets[i].isActive){
			drawBullet(&bullets[i], frameCount, background);
		}
	}
}
void generateBullets(bullet bullets[], uint8_t numOfBullets, vector_t enemies[], uint8_t numOfEnemies, vector_t *playerPos, uint16_t bulletSpeed){
	for(uint8_t i = 0; i < numOfEnemies; i++){
		if(!bullets[i].isActive){
			bullets[i].isActive = 1;
			bullets[i].pos = enemies[i];
			bullets[i].radius = 2 << FIX;
			bullets[i].anchor.x = 2;
			bullets[i].anchor.y = 2;
			vector_t direction = subtractVectors(playerPos, &enemies[i]);
			direction = normalizeFIXVector(&direction);
			bullets[i].vel = multFIXVector(&direction, bulletSpeed);
		}
	}
}

//----------------------------------POWERUPS-----------------------------------
int16_t getPowerupCountdown(){
	return 60 + (rand16bit() >> 10);
}
void generateNewPowerup(powerup powerups[], uint8_t numPowerups){
	for(uint8_t i = 0; i < numPowerups; i++){
		if(!powerups[i].isActive){
			powerups[i].framesLeft = 60 + (rand16bit() >> 10);
			powerups[i].pos.x = FIXMUL(rand16bit(), 0x170) + 0x600; //These are magic numbers to ensure the powerups are generated within bounds
			powerups[i].pos.y = FIXMUL(rand16bit(), 0x170) + 0x600;
			powerups[i].isActive = 1;

			//TODO VALDEMAR print powerup
			gotoxy(powerups[i].pos.x >> FIX, powerups[i].pos.y >> (FIX + 1));
			fgcolor(RED);
			bgcolor(WHITE);
			printf("X");
			break;
		}
	}
}

//--------------------------------DRAWING FUNCITONS---------------------------------------------
void updateSentryPos(sentry_t* sentry, uint32_t frameCount) {


	int32_t offsetX = FIXMUL(cosi(frameCount + sentry->phase), sentry->orbitRadius);
	int32_t offsetY = FIXMUL(sine(frameCount + sentry->phase), sentry->orbitRadius);

	sentry->pos.x = sentry->orbitPos.x + offsetX;
	sentry->pos.y = sentry->orbitPos.y + offsetY;

	debug1 = sentry->pos.x;
	debug2 = sentry->pos.y;
}


void drawPowerup(powerup* powerup, uint32_t frameCount, const uint8_t* background){
	drawSprite(background, Power_Up_Anim[frameCount/16 % 2], 3, 7, WHITE, FIX_2_X(powerup), FIX_2_Y(powerup));
}

void drawSentry(sentry_t* sentry, uint32_t frameCount, const uint8_t* background) {
	drawSprite(background, Sentry_Anim[frameCount/8 % 4], 4, 5, RED, FIX_2_X(sentry), FIX_2_Y(sentry));
}

void drawBullet(bullet* bullet, uint32_t frameCount, const uint8_t* background){
	drawSprite(background, Bullet_Anim[frameCount/8 % 3], 1, 2, WHITE, FIX_2_X(bullet), FIX_2_Y(bullet));
}

void drawAsteroid(GravitySource* asteroid, const uint8_t* background) {
	drawSprite(background, Asteroid_1, 5, 10, BROWN, FIX_2_X(asteroid), FIX_2_Y(asteroid));
}

void drawAlien(GravityTarget* alien, int alienNum, uint32_t frameCount, const uint8_t* background, uint8_t playerHit) {
	switch(alienNum) {
	case 1:
		drawSprite(background, playerHit ? Alien1_Dead: Alien1_Anim[frameCount/8 % 2], 3, 4, GREEN, FIX_2_X(alien), FIX_2_Y(alien));
		break;
	case 2:
		drawSprite(background, playerHit ? Alien2_Dead: Alien2_Anim[frameCount/8 % 2], 3, 4, GREEN, FIX_2_X(alien), FIX_2_Y(alien));
		break;
	case 3:
		drawSprite(background, playerHit ? Alien3_Dead: Alien3_Anim[frameCount/8 % 2], 2, 4, GREEN, FIX_2_X(alien), FIX_2_Y(alien));
		break;
	}
}

//----------------------------------LED-----------------------------------------
void setLEDToIndicateHealth(uint8_t livesLeft){

	if(livesLeft == 2)
		setLED(2); //Green
	else if(livesLeft == 1)
		setLED(1); //Red
	else
		setLED(5); //Pink unlimited health
}

//----------------------------------FRAME TIMER---------------------------------
void TIM1_BRK_TIM15_IRQHandler(void) {
	updateFrame = 1;
	resetTimer15();
}

//This interrupt handles BossScreen. Fix baggrund på return.
void EXTI4_IRQHandler(void) {
	//Pauses everything
	TIM15->DIER &= ~(0x0001);
	changeMusic(1);
	drawBackground(bossScreenBG);
	while((GPIOA->IDR)&(1<<4));
	while(!((GPIOA->IDR)&(1<<4)));
	for (uint32_t i = 0; i<1000000;i++);
	//Reenters prev state
	TIM15->DIER |= 0x0001;
	changeMusic(0);
	EXTI_ClearFlag(EXTI_Line4);
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void initBossScreen(void) {
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
	GPIOA->MODER &= ~(0x00000003 << (4 * 2));
	GPIOA->PUPDR &= ~(0x00000002 << (4 * 2));
	GPIOA->PUPDR |= (0x00000002 << (4 * 2));
	RCC->APB2ENR |= (1<<0);
	SYSCFG->EXTICR[1] = 0;

	EXTI->IMR 		|= (1 << 4);
	//EXTI->EMR  		|= (1 << 4);
	EXTI->RTSR      |= (1 << 4);

	NVIC_SetPriority(EXTI4_IRQn, 1);
	NVIC_EnableIRQ(EXTI4_IRQn);

}


