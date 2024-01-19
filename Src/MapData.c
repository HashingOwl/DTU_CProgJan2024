/*
 * MapData.c
 *
 *  Created on: 19. jan. 2024
 *      Author: valde
 */


//========================================MAP=1=========================================================
#include "MapData.h"

const sentry_t Sentry_Configuration_1[4] = {
		{.orbitPos = {U_WIDTH/2, U_HEIGHT/2}, 	.orbitRadius = 35 << FIX, 	.phase = 0, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
		{.orbitPos = {U_WIDTH/2, U_HEIGHT/2}, 	.orbitRadius = 35 << FIX, 	.phase = 512, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
		{.orbitPos = {160 << FIX, 50 << FIX}, 	.orbitRadius = 25 << FIX, 	.phase = 512, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
		{.orbitPos = {40 << FIX, 150 << FIX}, 	.orbitRadius = 25 << FIX, 	.phase = 512, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
};

const GravitySource Asteroid_Configuration_1[7] = {
		{.pos = {U_WIDTH/2, U_HEIGHT/2}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {40 << FIX, 50 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {40 << FIX, 150 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {160 << FIX, 50 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {160 << FIX, 150 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {100 << FIX, 30 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {100 << FIX, 170 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
};

const map_t Map_1 = {
		.numAsteroids = 7,
		.numSentries = 4,
		.asteroidConfiguration = Asteroid_Configuration_1,
		.sentryConfiguration = Sentry_Configuration_1,
};

//========================================MAP=2=========================================================

const sentry_t Sentry_Configuration_2[4] = {
		{.orbitPos = {U_WIDTH/2, U_HEIGHT/2}, 	.orbitRadius = 30 << FIX, 	.phase = 0, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
		{.orbitPos = {U_WIDTH/2, U_HEIGHT/2}, 	.orbitRadius = 30 << FIX, 	.phase = 512, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
		{.orbitPos = {U_WIDTH/2, U_HEIGHT/2}, 	.orbitRadius = 80 << FIX, 	.phase = 0, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
		{.orbitPos = {U_WIDTH/2, U_HEIGHT/2}, 	.orbitRadius = 80 << FIX, 	.phase = 512, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
};

const GravitySource Asteroid_Configuration_2[7] = {
		{.pos = {U_WIDTH/2, U_HEIGHT/2}, 				.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {(100-39) << FIX, (100-39) << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {(100-39) << FIX, (100+39) << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {(100+39) << FIX, (100-39) << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {(100+39) << FIX, (100+39) << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {(100-85) << FIX, (100+85) << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {(100+85) << FIX, (100-85) << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
};


const map_t Map_2 = {
		.numAsteroids = 7,
		.numSentries = 4,
		.asteroidConfiguration = Asteroid_Configuration_2,
		.sentryConfiguration = Sentry_Configuration_2,
};

//========================================MAP=3=========================================================

const sentry_t Sentry_Configuration_3[4] = {
		{.orbitPos = {100 << FIX, 40 << FIX}, 	.orbitRadius = 25 << FIX, 	.phase = 0, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
		{.orbitPos = {100 << FIX, 160 << FIX}, 	.orbitRadius = 25 << FIX, 	.phase = 0, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
		{.orbitPos = {40 << FIX, 100 << FIX}, 	.orbitRadius = 25 << FIX, 	.phase = 0, 	.anchor = {10, 4}, 	.radius = 10 << FIX},
		{.orbitPos = {160 << FIX, 100 << FIX}, 	.orbitRadius = 25 << FIX, 	.phase = 0, 	.anchor = {10, 4}, 	.radius = 10 << FIX},

};

const GravitySource Asteroid_Configuration_3[5] = {
		{.pos = {100 << FIX, 40 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {100 << FIX, 160 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {40 << FIX, 100 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {160 << FIX, 100 << FIX}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 80 << 24},
		{.pos = {U_WIDTH/2, U_HEIGHT/2}, 	.anchor = {10, 10}, .radius = 12 << FIX, .mass = 120 << 24},
};


const map_t Map_3 = {
		.numAsteroids = 5,
		.numSentries = 4,
		.asteroidConfiguration = Asteroid_Configuration_3,
		.sentryConfiguration = Sentry_Configuration_3,
};

const map_t* Maps[3] = {
		&Map_1,
		&Map_2,
		&Map_3,
};
