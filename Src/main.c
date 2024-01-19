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
#include "lcd.h"
#include "bossScreen.h"

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

#define FIX_2_XStat(a) ((a.pos.x >> FIX) - a.anchor.x)
#define FIX_2_YStat(a) ((a.pos.y >> FIX) - a.anchor.y)

#define healthPU 0
#define invincibilityPU 1
#define speedPU 2
#define gravityPU 3

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
	uint32_t gameState = MENU;
	uint32_t frameCount = 0; // Used in sprite animations

	//ALIENS LEFT THROUGHOUT GAME
	uint8_t aliensLeftStart = 3; //Max number of aliens to die before game over
	uint8_t aliensLeft = aliensLeftStart;
	uint8_t aliensThrough = 0; //score

	//----------PLAYER ALIEN---------
	vector_t playerStartPos = {10 << FIX, 10<<FIX};
	GravityTarget ship = {
			.pos = playerStartPos,
			.vel = {0, 0},
			.anchor = {6, 4},
			.radius = 5 << FIX};
	//ALIEN HEALTH PR. INDIVIDUAL ALINE
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
	// Sentries
	int numSentries = 2;
	sentry_t sentries[2] = {
			{.orbitPos = {U_WIDTH/2, U_HEIGHT/2}, 	.orbitRadius = 40 << FIX, 	.phase = 0, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
			{.orbitPos = {U_WIDTH/2, U_HEIGHT/2}, 	.orbitRadius = 40 << FIX, 	.phase = 512, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
	};
	for (int i = 0; i < numSentries; i++) { updateSentryPos(&sentries[i], 0); }

	int16_t enemyShootResetValue = 20 * 5; // 20 * seconds between shoot
	int16_t enemyShootCountdown = enemyShootResetValue;

	//----------BULLETS--------------
	uint8_t numBullets = 20;
	bullet bullets[20] = {};
	uint16_t bulletSpeed = 0x500;
	uint16_t bulletSpeedIncrease = 0x60;
	uint32_t bulletRespawnRate = 0x100;

	//----------POWERUPS--------------
	char RNGSeedSet = 0;
	uint16_t powerupCountdown = getPowerupCountdown();
	powerup powerups[4] = {};
	uint8_t numPowerups = 4;
	uint8_t powerupEffects[4] = {};

	//----------MENU------------------
	uint8_t joyVal;
	uint8_t joyValPrev;
	int8_t currSelectionMainMenu = 0;
	uint8_t buttonLift = 1;

	//----------CONSOLE AND GRAPHICS--------------
	uint8_t backgroundContamination[P_WIDTH * P_HEIGHT / 8] = {};
	const uint8_t* currentBackground; // Not actually a constant, it can be changed in software. It's a pointer to a constant.

	//----------INITIALISATION OF HARD- AND SOFTWARE----------
	// Output
	uart_init(256000);
	initLED();
	setLED(BLUE);
	lcd_init();

	//while(1);
	//Input
	initAnalogJoystick();
	initDigitalJoystick(); // While using mini joystick as substitute for proper joystick
	joyVal = readAnalogJoystickDigital();
	joyValPrev = joyVal;

	//Music
	soundInit();
	//changeMusic(0);

	initTimer15(639, 5000);

	//Miscenlaneous
	initBossScreen();
	resetGrid(backgroundContamination);


	if(gameState == PLAYING){
		currentBackground = BG_Stratosphere_2;
		initGame(currentBackground, asteroids, numAsteroids, livesLeft);
	}else if (gameState == MENU){
		currentBackground = MainMenuBG;
		drawBackground(currentBackground);
	}

	//Init LCD-display
	uint8_t bufferLCD[512];
	clearLCDBuffer(bufferLCD);
	//resetScore();
	drawScore(bufferLCD, 0,0);
	drawScore(bufferLCD, readHighscore(),1);
	addLivesBuffer(bufferLCD,3);
	drawLCD(bufferLCD);

	void resetGameVals(){
		aliensLeft = aliensLeftStart;
		aliensThrough = 0;
		ship.pos = playerStartPos;
		ship.vel.x = 0;
		ship.vel.y = 0;
		livesLeft = maxLives;
		currentAlien = 1;
		enemyShootCountdown = enemyShootResetValue;
		for (int i = 0; i < numSentries; i++) { sentries[i].orbitPos.x=U_WIDTH/2; sentries[i].orbitPos.y= U_HEIGHT/2;  sentries[i].phase = 512*i; }
		for (int i= 0; i < numBullets; i++) {bullets[i].isActive = 0;}
		for (int i= 0; i < numPowerups; i++) {powerups[i].isActive = 0;}
	}

	//MAIN LOOP
	while(1){
		if(updateFrame){
			updateFrame = 0;
			// Frame count updated in bottom of loop
			switch (gameState) {
//==========================================MENU========================================================
			case MENU:
				// Read input
				joyValPrev = joyVal;
				joyVal = readAnalogJoystickDigital();
				// Joystick changed, move selection
				if (joyVal != joyValPrev) {
					// clear old aliens
					drawBackgroundRect(currentBackground, 14, 90 + currSelectionMainMenu * 38, 26+12, 8);
					drawBackgroundRect(currentBackground, 147, 90 + currSelectionMainMenu * 38, 26+12, 8);

					for(uint8_t i = 0;i < 2; i++){
						//contaminateRect(backgroundContamination,46+i*95,90+currSelectionMainMenu*38,12,12);
					}

					if (joyVal & 0b1) {
						currSelectionMainMenu--;
					}
					else if ( (joyVal & 0b10) && joyVal != 0) {
						currSelectionMainMenu++;
					}

					if (currSelectionMainMenu >2) {
						currSelectionMainMenu = 0;
					}
					else if(currSelectionMainMenu < 0) {
						currSelectionMainMenu = 2;
					}

					// Draw new aliens
					drawSprite(currentBackground, Alien1_1, 3, 4, BLUE, 40, 90 + currSelectionMainMenu * 38);
					drawSprite(currentBackground, Alien2_1, 3, 4, GREEN, 25, 90 + currSelectionMainMenu * 38);
					drawSprite(currentBackground, Alien3_1, 2, 4, RED, 14, 90 + currSelectionMainMenu * 38);

					drawSprite(currentBackground, Alien1_1, 3, 4, BLUE, 147, 90 + currSelectionMainMenu * 38);
					drawSprite(currentBackground, Alien2_1, 3, 4, GREEN, 162, 90 + currSelectionMainMenu * 38);
					drawSprite(currentBackground, Alien3_1, 2, 4, RED, 177, 90 + currSelectionMainMenu * 38);
				}

				if (frameCount == 0) {
					drawSprite(currentBackground, Alien1_1, 3, 4, BLUE, 40, 90 + currSelectionMainMenu * 38);
					drawSprite(currentBackground, Alien2_1, 3, 4, GREEN, 25, 90 + currSelectionMainMenu * 38);
					drawSprite(currentBackground, Alien3_1, 2, 4, RED, 14, 90 + currSelectionMainMenu * 38);

					drawSprite(currentBackground, Alien1_1, 3, 4, BLUE, 147, 90 + currSelectionMainMenu * 38);
					drawSprite(currentBackground, Alien2_1, 3, 4, GREEN, 162, 90 + currSelectionMainMenu * 38);
					drawSprite(currentBackground, Alien3_1, 2, 4, RED, 177, 90 + currSelectionMainMenu * 38);
				}
				/*
				drawCleanBackground(currentBackground, backgroundContamination);
				resetGrid(backgroundContamination);

				for(uint8_t i = 0;i < 2; i++){
					drawSprite(currentBackground, Alien1_Anim[0], 3, 4, WHITE, 46+i*95,90+currSelectionMainMenu*38);
				}
				*/

				// Button pressed down, select this option
				if (readJoystickButtons() && buttonLift) {
					buttonLift = 0;
					playBeep();
					switch(currSelectionMainMenu) {
						//Goto game
						case 0:
							resetGameVals();
							currentBackground = BG_Stratosphere_2;
							gameState = PLAYING;
							frameCount = 0;
							initGame(currentBackground, asteroids, numAsteroids, livesLeft);
							break;
						//Goto help screen
						case 1:
							currentBackground = HelpScreen;
							drawBackground(currentBackground);
							gameState = HELP;
							frameCount = 0;
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

				if(playerHit > 0)
					playerHit--;

				//-------------------INPUT----------------------------------
				//Getting input from joystick. Passed as reference
				vector_t input = {0, 0};
				readAnalogJoystick(&input.x, &input.y);
				// Temporary when testing from a board without proper joystick. Please outcomment it instead of deleting.
				/*
				joyVal = readDigitalJoystick();
				input.y -= ((joyVal & 1) != 0) * 160;
				input.y += ((joyVal & 2) != 0) * 160;
				input.x -= ((joyVal & 4) != 0) * 160;
				input.x += ((joyVal & 8) != 0) * 160;*/

				//------------------PLAYER PHYSICS---------------------------------
				//Update velocity based on input and Powerup
				if(powerupEffects[speedPU]){
					input.x = FIXMUL(input.x, 0x1A0);
					input.y = FIXMUL(input.y, 0x1A0);
				}
				ship.vel.x += input.x;
				ship.vel.y += input.y;

				if(!powerupEffects[gravityPU]) //Only apply gravity if antigravity powerup is NOT active;
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
					if(ship.pos.x > U_WIDTH - ship.radius - 0x2000){ //Win condition
						playBeep();
						bulletSpeed += bulletSpeedIncrease;
						bulletRespawnRate = FIXMUL(bulletRespawnRate, 0xA0);
						livesLeft = maxLives;
						setLEDToIndicateHealth(livesLeft);

						aliensThrough++;
						drawScore(bufferLCD, aliensThrough, 0);
						drawLCD(bufferLCD);
						makeNewAlien(&ship, &playerStartPos, &currentAlien);
					}else{
						ship.pos.y = U_WIDTH - ship.radius - BORDER_PAD;
						ship.vel.y /= -4;
					}
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
				enemyShootCountdown--;
				// Update sentry positions
				for (int i = 0; i < numSentries; i++) {
					updateSentryPos(&sentries[i], frameCount);
				}

				// Draw Sentries
				for (int i = 0; i < numSentries; i++) {
					drawSentry(&sentries[i], frameCount, currentBackground);
					cleanRect(backgroundContamination, (sentries[i].pos.x >> FIX) - sentries[i].anchor.x, (sentries[i].pos.y >> FIX) - sentries[i].anchor.y, 16, 10);
				}

				// Apply gravity to bullets
				for(uint8_t b = 0; b < numBullets; b++){
					applyGravity(bullets[b].pos, &bullets[b].vel, asteroids, numAsteroids);
				}

				// Update bullet position
				bulletUpdatePosition(bullets, numBullets, asteroids, numAsteroids);

				// Check and handle bullet collision with player
				if(!powerupEffects[invincibilityPU]){
					char hitBullet = bulletHitPlayer(&ship.pos, bullets, numBullets);
					char hitSentry = sentryHitPlayer(&ship, sentries, numSentries);
					if(hitBullet || hitSentry){
						playBeep();
						if(livesLeft == 1 || hitSentry){ //Alien dies
							aliensLeft--;
							livesLeft = maxLives;

							addLivesBuffer(bufferLCD, aliensLeft); //Draw to LCD
							drawLCD(bufferLCD);
							makeNewAlien(&ship, &playerStartPos, &currentAlien);
							if (aliensLeft == 0) { //Game over
								gameState = MENU;
							}
						}else{ //Player took hit but did not die
							livesLeft--;
							playerHit = 25;
						}
						setLEDToIndicateHealth(livesLeft);
					}
				}

				// Draw bullets
				drawAllBullets(bullets, numBullets, frameCount, currentBackground);

				// Clean Bullets
				for (int i = 0; i < numBullets; i++) {
					if (bullets[i].isActive) {
						cleanRect(backgroundContamination, (bullets[i].pos.x >> FIX) - bullets[i].anchor.x, (bullets[i].pos.y >> FIX) - bullets[i].anchor.y, 4, 4);
					}
				}

				// Generate bullets
				if(!enemyShootCountdown){
					generateBullets(bullets, numBullets, sentries, numSentries, &ship.pos, bulletSpeed);
					enemyShootCountdown = getBulletCountdown(bulletRespawnRate);
				}

				//--------------------------POWERUPS---------------------------------
				//Decreasing powerup effect countdowns and drawing powerups
				powerupCountdown--;
				for(uint8_t i = 0; i < 4; i++){
					//Decresing powerupeffect countdown and applying end effect
					if(powerupEffects[i]){
						powerupEffects[i]--;
						if((i == invincibilityPU) && (!powerupEffects[i])) //Invincibility powerup just ended.
							setLEDToIndicateHealth(livesLeft);
					}

					//Draw active powerups but only every 8th frame to preserve putty bandwith
					if(powerups[i].isActive && !(frameCount & 0b111))
						drawPowerup(&powerups[i], frameCount, currentBackground);
				}

				//Generating powerups
				if(powerupCountdown == 0){
					if(!RNGSeedSet){
						RNGSeedSet = 1;
						setRNGSeed((ship.pos.x & 0x000000FF) * (ship.pos.y & 0x000000FF)); //Semi random way to initialize RNG
					}
					generateNewPowerup(powerups, numPowerups, frameCount, currentBackground, asteroids, numAsteroids);
					powerupCountdown = getPowerupCountdown();
				}

				//Checking for colliossn with powerups AND START POWERUP EFFECT
				for(uint8_t p = 0; p < numPowerups; p++){
					vector_t anchorPos = {ship.pos.x - 0x400, ship.pos.y - 0x400};
					if(circleCollision(&powerups[p].pos, &anchorPos, 0x7500)){
						playBeep();
						powerups[p].isActive = 0;
						contaminateRect(backgroundContamination, FIX_2_XStat(powerups[p]), FIX_2_YStat(powerups[p]), 3*4, 7*2);

						switch (powerups[p].power){
						case healthPU:
							livesLeft = maxLives;
							if(aliensLeft < aliensLeftStart){
								aliensLeft++;
								addLivesBuffer(bufferLCD, aliensLeft);
								drawLCD(bufferLCD);
							}
							setLEDToIndicateHealth(livesLeft);
						break;
						case invincibilityPU:
							powerupEffects[powerups[p].power] = getPowerupCountdown() * 3;
							setLED(PURPLE);
							break;
						case speedPU:
						case gravityPU:
							powerupEffects[powerups[p].power] = getPowerupCountdown() * 3;
						break;
						}
					}
				}

				//-------------------Drawing--------------------------------
				// Player
				drawAlien(&ship, currentAlien, frameCount, currentBackground, playerHit);
				cleanRect(backgroundContamination, (ship.pos.x >> FIX) - ship.anchor.x, (ship.pos.y >> FIX) - ship.anchor.y, 12, 8);

				// Clean Background
				drawCleanBackground(currentBackground, backgroundContamination);
				resetGrid(backgroundContamination);

				//------------Contaminate-for-next-frame--------------------
				// Player
				contaminateRect(backgroundContamination, (ship.pos.x >> FIX) - ship.anchor.x, (ship.pos.y >> FIX) - ship.anchor.y, 12, 8);
				// Contaminate Sentries
				for (int i = 0; i < numSentries; i++) {
					contaminateRect(backgroundContamination, (sentries[i].pos.x >> FIX) - sentries[i].anchor.x, (sentries[i].pos.y >> FIX) - sentries[i].anchor.y, 16, 10);
				}
				// Bullet
				for (int i = 0; i < numBullets; i++) {
					if (bullets[i].isActive) {
						contaminateRect(backgroundContamination, (bullets[i].pos.x >> FIX) - bullets[i].anchor.x, (bullets[i].pos.y >> FIX) - bullets[i].anchor.y, 4, 4);
					}
				}
				if (readJoystickButtons() && buttonLift) {
						playBeep();
						buttonLift = 0;
						gameState = MENU;
				}
				else if (readJoystickButtons() == 0){
					buttonLift = 1;
				}
				if (gameState == MENU) {
					currentBackground = MainMenuBG;
					drawBackground(currentBackground);
					frameCount = 0;
					continue;
				}
				break;
//=========================================HELP======================================================
			case HELP:
				// Check for input, showing either boss_bakcground, or changing state to MENU or PLAYING
				if (readJoystickButtons() && buttonLift) {
					playBeep();
					drawScore(bufferLCD,aliensThrough,0);
					buttonLift = 0;
					currentBackground = MainMenuBG;
					drawBackground(currentBackground);
					gameState = MENU;
					frameCount = 0;
					continue;
				}
				else if (readJoystickButtons() == 0){
					buttonLift = 1;
				};
			}
			if (bossScreenReturnFlag){
				bossScreenReturnFlag=0;
				drawBackground(currentBackground);
				if (gameState == PLAYING) {
					initGame(currentBackground, asteroids, numAsteroids, livesLeft);
				}
			}
		// Update frame count
		frameCount++;
		}
	}
}

//----------------------------------GAME CONTROL----------------------------
void initGame(const uint8_t* currentBackground, GravitySource asteroids[], uint8_t numAsteroids, uint8_t livesLeft){
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
	vector_t anchorPos = *playerPos;
	anchorPos.x -= 0x600;
	anchorPos.y -= 0x400;
	for (uint8_t i = 0; i < numBullets; i++)
	{
		if(bullets[i].isActive){
			if(circleCollision(playerPos, &bullets[i].pos, 0x3A00)){
				bullets[i].isActive = 0;
				return 1;
			}
		}
	}
	return 0;
}

char sentryHitPlayer(GravityTarget* ship, sentry_t sentries[], uint8_t numSentries){
//	vector_t anchorPos;
//	anchorPos = subtractVectors(&(ship->pos), &(ship->anchor));
//	anchorPos = addVectors(&anchorPos, &(sentries[0].anchor));
	for (uint8_t i = 0; i < numSentries; i++)
	{
		if(circleCollision(&(ship->pos), &sentries[i].pos, 0x4A00)){
			return 1;
		}
	}
	return 0;
}

void drawAllBullets(bullet bullets[], uint8_t numOfBullets, uint32_t frameCount, const uint8_t* background){
	for(uint8_t i = 0; i < numOfBullets; i++){
		if(bullets[i].isActive){
			drawBullet(&bullets[i], frameCount, background);
		}
	}
}
void generateBullets(bullet bullets[], uint8_t numOfBullets, sentry_t* enemies, uint8_t numOfEnemies, vector_t *playerPos, uint16_t bulletSpeed){
	for(uint8_t i = 0; i < numOfEnemies; i++){
		if(!bullets[i].isActive){
			bullets[i].isActive = 1;
			bullets[i].pos = enemies[i].pos;
			bullets[i].radius = 2 << FIX;
			bullets[i].anchor.x = 2;
			bullets[i].anchor.y = 2;
			vector_t direction = subtractVectors(playerPos, &(enemies[i].pos));
			direction = normalizeFIXVector(&direction);
			bullets[i].vel = multFIXVector(&direction, bulletSpeed);
		}
	}
}
int16_t getBulletCountdown(int32_t bulletSpeed){
	int16_t time = FIXMUL(10 + (rand16bit() >> 12), bulletSpeed);
	if(time < 10)
		time = 10;
	return time;
}

//----------------------------------POWERUPS-----------------------------------
int16_t getPowerupCountdown(){
	int16_t time = 50 + (rand16bit() >> 8);
	if(time < 30)
		time = 30;
	return time;
}
void generateNewPowerup(powerup powerups[], uint8_t numPowerups, uint16_t frameCount, const uint8_t currentbackground[], GravitySource asteroids[], uint8_t numAsteroids){
	void randomVector(vector_t* pos){
		pos->x = FIXMUL(rand16bit(), 0x160) + 0x600;
		pos->y = FIXMUL(rand16bit(), 0x160) + 0x600;
	}
	char collisionWithAsteroids(vector_t* pos){
		for(uint8_t i = 0; i < numAsteroids; i++){
			if(circleCollision(pos, &asteroids[i].pos, 0x20000))
				return 1;
		}
		return 0;
	}

	for(uint8_t i = 0; i < numPowerups; i++){
		if(!powerups[i].isActive){
			powerups[i].framesLeft = 60 + (rand16bit() >> 10);

			vector_t pos;
			randomVector(&pos);
			while(collisionWithAsteroids(&pos)) //Making sure powerup does not spawn on asteroid
				randomVector(&pos);

			powerups[i].pos.x = pos.x; //These are magic numbers to ensure the powerups are generated within bounds
			powerups[i].pos.y = pos.y;
			powerups[i].isActive = 1;
			powerups[i].power = powerups[i].pos.x & 0b11; //Semi random method of choosing power
			powerups[i].anchor.x = 6;
			powerups[i].anchor.y = 7;
			break;
		}
	}
}

//--------------------------------DRAWING FUNCITONS---------------------------------------------
void updateSentryPos(sentry_t* sentry, uint32_t frameCount) {


	int32_t offsetX = FIXMUL(cosi((frameCount<<2) + sentry->phase), sentry->orbitRadius);
	int32_t offsetY = FIXMUL(sine((frameCount<<2) + sentry->phase), sentry->orbitRadius);

	sentry->pos.x = sentry->orbitPos.x + offsetX;
	sentry->pos.y = sentry->orbitPos.y + offsetY;

	debug1 = sentry->pos.x;
	debug2 = sentry->pos.y;
}


void drawPowerup(powerup* powerup, uint32_t frameCount, const uint8_t* background){
	uint8_t color;
	switch (powerup->power){
	case healthPU:
		color = RED; break;
	case invincibilityPU:
		color = PURPLE; break;
	case speedPU:
		color = BLUE; break;
	case gravityPU:
		color = GREEN; break;
	}
	drawSprite(background, Power_Up_Anim[frameCount/16 % 2], 3, 7, color, FIX_2_X(powerup), FIX_2_Y(powerup));
}

void drawSentry(sentry_t* sentry, uint32_t frameCount, const uint8_t* background) {
	drawSprite(background, Sentry_Anim[frameCount/8 % 4], 4, 5, RED, FIX_2_X(sentry), FIX_2_Y(sentry));
}

void drawBullet(bullet* bullet, uint32_t frameCount, const uint8_t* background){
	drawSprite(background, Bullet_Anim[frameCount/2 % 3], 1, 2, WHITE, FIX_2_X(bullet), FIX_2_Y(bullet));
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




