/*
 * Graphics.c
 *
 *  Created on: 15. jan. 2024
 *      Author: valde
 */

#include "graphics.h"

/*
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include <stdio.h>
#include "timer.h"
#include "GraphicData.h"
#include "joystick.h"
#include <string.h>
*/

#define ESC 0x1B

#define PIX_NONE 32
#define PIX_TOP 223
#define PIX_BOT 220
#define PIX_FULL 219

#define BLACK 0
#define RED 1
#define GREEN 2
#define BLUE 4
#define PURPLE 5
#define GRAY 7
#define YELLOW 11
#define WHITE 15

#define BACKGROUND_COLOR GRAY

// Experimental, should be faster
void print_byte(uint8_t byte) {
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, byte);
}

void print_bytes(uint8_t* bytes, uint8_t numBytes) {
	for (uint8_t i = 0; i < numBytes; i++) {
		print_byte(bytes[i]);
	}
}

// Upper left corner is (0, 0)
void gotoxy(uint8_t x, uint8_t y) {
	x++; y++;

	print_byte(ESC);
	print_byte((uint8_t)'[');

	if (y>=100) { print_byte((y/100) + 48); }
	if (y>=10) 	{ print_byte(((y/10)%10) + 48); }
	print_byte((y%10) + 48);

	print_byte((uint8_t)';');

	if (x>=100) { print_byte((x/100) + 48); }
	if (x>=10) 	{ print_byte(((x/10)%10) + 48); }
	print_byte((x%10) + 48);

	print_byte((uint8_t)'H');
}

void fgcolor(uint8_t foreground) {
	if (foreground > 7) {
		foreground -= 8;
		print_byte(ESC);
		print_byte((uint8_t)'[');
		print_byte((uint8_t)'1'); // Type = 1
		print_byte((uint8_t)';');
		print_byte((uint8_t)'3'); // 3x
		print_byte(foreground + 48);
		print_byte((uint8_t)'m');
	} else {
		print_byte(ESC);
		print_byte((uint8_t)'[');
		print_byte((uint8_t)'2'); // Type = 22
		print_byte((uint8_t)'2');
		print_byte((uint8_t)';');
		print_byte((uint8_t)'3'); // 3
		print_byte(foreground + 48); // ASCI C850 offset
		print_byte((uint8_t)'m');
  }

}

void bgcolor(uint8_t background) {
	print_byte(ESC);
	print_byte((uint8_t)'[');
	print_byte((uint8_t)'4'); // 4x
	print_byte(background + 48); // ASCI C850 offset
	print_byte((uint8_t)'m');

}

void color(uint8_t foreground, uint8_t background) {
	if (foreground > 7) {
		foreground -= 8;
		print_byte(ESC);
		print_byte((uint8_t)'[');
		print_byte((uint8_t)'1'); // Type = 1
		print_byte((uint8_t)';');
		print_byte((uint8_t)'3'); // 3x
		print_byte(foreground + 48);
		print_byte((uint8_t)';');
		print_byte((uint8_t)'4'); // 3x
		print_byte(background + 48);
		print_byte((uint8_t)'m');
	} else {
		print_byte(ESC);
		print_byte((uint8_t)'[');
		print_byte((uint8_t)'2'); // Type = 22
		print_byte((uint8_t)'2');
		print_byte((uint8_t)';');
		print_byte((uint8_t)'3'); // 3
		print_byte(foreground + 48); // ASCI C850 offset
		print_byte((uint8_t)';');
		print_byte((uint8_t)'4'); // 3x
		print_byte(background + 48);
		print_byte((uint8_t)'m');
	}
}
// Used for drawing sprites wiht odd y-cords
uint8_t getTopPaddedByte(const uint8_t* imageData, uint8_t byteIndex) {
	return ((imageData[byteIndex] << 1) & 0xAA);
}

uint8_t getBottomPaddedByte(const uint8_t* imageData, uint8_t byteIndex) {
	return ((imageData[byteIndex] >> 1) & 0x55);
}

// TO DO: Is &3 faster than %4 ? This should be investigated for entire document.
// xPos, yPos and width in pixels
uint8_t getByteAtPos(const uint8_t* imageData, uint8_t xPos, uint8_t yPos, uint8_t width) {
	uint8_t dx = xPos % 4;
	uint8_t dy = yPos % 2;
	xPos -= dx;
	yPos -= dy;
	uint8_t leftByte = imageData[(yPos/2)*width/4 + (xPos/4)];
	uint8_t rightByte = imageData[(yPos/2)*width/4 + (xPos/4) + 1];
	uint8_t topByte = (rightByte << (8 - 2*dx)) | (leftByte >> (2*dx));
	if (dy == 1) {
		uint8_t leftByte2 = imageData[(yPos/2+1)*width/4 + (xPos/4)];
		uint8_t rightByte2 = imageData[(yPos/2+1)*width/4 + (xPos/4) + 1];
		uint8_t bottomByte = (rightByte2 << (2*(4-dx))) | (leftByte2 >> (2*dx));
		uint8_t result = ((topByte >> 1) & 0x55) | ((bottomByte << 1) & 0xAA);
		return result;
	} else {
		return topByte;
	}
}

// Find alt to 8-bit 16-bit
void drawBgChar(uint8_t byte, uint8_t i) {
	switch ((byte >> (2*i)) & 0x03) {
	case 0b00:
		print_byte(PIX_NONE);
		break;
	case 0b01:
		print_byte(PIX_TOP);
		break;
	case 0b10:
		print_byte(PIX_BOT);
		break;
	case 0b11:
		print_byte(PIX_FULL);
		break;
	}
}

// This func must die...
void drawBgChar16(uint16_t byte, uint8_t i) {
	switch ((byte >> (2*i)) & 0x03) {
	case 0b00:
		print_byte(PIX_NONE);
		break;
	case 0b01:
		print_byte(PIX_TOP);
		break;
	case 0b10:
		print_byte(PIX_BOT);
		break;
	case 0b11:
		print_byte(PIX_FULL);
		break;
	}
}

void drawBackground(const uint8_t* background) {
	// Background assumed formatted as four characters, 4 x 2 pixels
	color(BACKGROUND_COLOR, BLACK);
	gotoxy(0,0);
	for (uint16_t b = 0; b < P_WIDTH*P_HEIGHT/4; b++) {
		uint8_t byte = background[b];
		for (uint8_t i = 0; i < 4; i++) {
			drawBgChar(byte, i);
		}
	}
}

// TO DO: Reconsider name...
// Global variable (!!!) Why? Speed is key, passing it around would be time consuming (Yes, really.)
uint8_t colorMode = 0; // 0: Background, Black. 1: Sprite, Black. 2: Sprite, Background

// TO DO: Optimize color() UART
// Draws a byte on a background
void drawSpriteByte(uint8_t spriteByte, uint8_t bgByte, uint8_t spriteColor) {
	//static uint8_t colorMode = 0; // 0: Background, Black. 1: Sprite, Black. 2: Sprite, Background
	for (uint8_t i = 0; i < 4; i++) {
		switch ((spriteByte >> (2*i)) & 0x03) {
		case 0b00:
			// Draw background here
			if (colorMode != 0) {
				color(BACKGROUND_COLOR, BLACK);
				colorMode = 0;
			}
			drawBgChar(bgByte, i);
			break;
		case 0b01:
			if (((bgByte) >> (2*i) & 0b10) == 0b10) {
				if (colorMode != 2) {
					color(spriteColor, BACKGROUND_COLOR);
					colorMode = 2;
				}
			} else if (colorMode != 1) {
				color(spriteColor, BLACK);
				colorMode = 1;
			}
			print_byte(PIX_TOP);
			break;
		case 0b10:
			if (((bgByte) >> (2*i) & 0b01) == 0b01) {
				if (colorMode != 2) {
					color(spriteColor, BACKGROUND_COLOR);
					colorMode = 2;
				}
			} else if (colorMode != 1) {
				color(spriteColor, BLACK);
				colorMode = 1;
			}
			print_byte(PIX_BOT);
			break;
		case 0b11:
			if (colorMode != 1) {
				color(spriteColor, BLACK);
				colorMode = 1;
			}
			print_byte(PIX_FULL);
			break;
		}
	}
}

// Draws a sprite at a given location
// Upper left corner is (0,0). Position is in pixels, NOT PuTTY coords.
void drawSprite(const uint8_t* background, const uint8_t* spriteData, uint8_t spriteWidth, uint8_t spriteHeight, uint8_t spriteColor, uint8_t xPos, uint8_t yPos) {
	// Prepare colorMode
	colorMode = 0;
	color(BACKGROUND_COLOR, BLACK);
	uint8_t yDelt = (yPos % 2); // 0 or 1
	if (yDelt == 1) { 	// When drawing on uneven pixel row, on a half PuTTY row
		// Draw initial top-padded row
		gotoxy(xPos, yPos/2);
		for (uint8_t i = 0; i < spriteWidth; i++) {
			uint8_t bgByte =  getByteAtPos(background, xPos+4*i, yPos-1, P_WIDTH);
			uint8_t spriteByte = getTopPaddedByte(spriteData, i);
			drawSpriteByte(spriteByte, bgByte, spriteColor);
		}
		// x and y is for bytes, so 4 and 2 respectively
		for (uint8_t y = 0; y < spriteHeight-1; y++) {
			gotoxy(xPos, (yPos/2)+y+1);
			for (uint8_t x = 0; x < spriteWidth; x++) {
				uint8_t bgByte =  getByteAtPos(background, xPos+4*x, yPos + 2*y + 1, P_WIDTH);
				uint8_t spriteByte = getByteAtPos(spriteData, x*4, y*2 + 1, spriteWidth*4);
				drawSpriteByte(spriteByte, bgByte, spriteColor);
			}
		}
		// Draw final bottom-padded row
		gotoxy(xPos, yPos/2 + spriteHeight);
		for (uint8_t i = 0; i < spriteWidth; i++) {
			uint8_t bgByte =  getByteAtPos(background, xPos+4*i, yPos + 2*(spriteHeight) - 1, P_WIDTH);
			uint8_t spriteByte = getBottomPaddedByte(spriteData, (spriteHeight-1)*(spriteWidth) + i);
			drawSpriteByte(spriteByte, bgByte, spriteColor);
		}
	} else { 	// When drawing "normally"
		// x and y is for bytes, so 4 and 2 respectively
		for (uint8_t y = 0; y < spriteHeight; y++) {
			gotoxy(xPos, (yPos/2)+y);
			for (uint8_t x = 0; x < spriteWidth; x++) {
				uint8_t bgByte =  getByteAtPos(background, xPos+4*x, yPos + 2*y, P_WIDTH);
				uint8_t spriteByte = spriteData[y*(spriteWidth)+x];
				drawSpriteByte(spriteByte, bgByte, spriteColor);
			}
		}
	}

}

void resetGrid(uint8_t* grid) {
	memset(grid, 0, P_WIDTH*P_HEIGHT/8);
}

// TO DO: Optimize for speed
// All in pixel coordinates
void contaminateRect(uint8_t* grid, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	for (uint8_t r = y/2; r < (y+h+1)/2; r++) {
		for (uint8_t c = x; c < x+w; c++) {
			grid[r*P_WIDTH/8 + c/8] |= (0b1 << c%8);
		}
	}
}

// TO DO: Optimize for speed
// All in pixel coordinates
void cleanRect(uint8_t* grid, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	for (uint8_t r = y/2; r < (y+h+1)/2; r++) {
		for (uint8_t c = x; c < x+w; c++) {
			grid[r*P_WIDTH/8 + c/8] &= ~(0b1 << c%8);
		}
	}
}

void drawCleanBackground(const uint8_t* background, uint8_t* cleanGrid) {
	color(BACKGROUND_COLOR, BLACK);
	// x and y in PuTTY coords
	uint8_t cleanByte;
	uint8_t skipping = 0;
	for (uint8_t row = 0; row < P_HEIGHT; row++) {
		for (uint8_t x = 0; x < P_WIDTH; x+=8) { // We asume (P_WIDTH % 8 == 0)
			cleanByte = cleanGrid[(row*P_WIDTH+x)/8];
			if (cleanByte == 0) { // Most often
				skipping = 1;
				continue;
			}
			uint16_t bg = background[row*P_WIDTH/4 + x/4] | (background[row*P_WIDTH/4+ x/4 +1] << 8);
			for (uint8_t i = 0; i < 8; i++) {
				if ((cleanByte >> i) & 0b1) {
					if (skipping) {
						gotoxy(x+i, row);
						skipping = 0;
					}
					gotoxy(x+i, row);
					drawBgChar16(bg, i);
				} else {
					skipping = 1;
				}
			}

		}
	}
}
