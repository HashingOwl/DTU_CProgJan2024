/*
 * graphics.h
 *
 *  Created on: 15. jan. 2024
 *      Author: valde
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

/*
 * Graphics.c
 *
 *  Created on: 15. jan. 2024
 *      Author: valde
 */

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include <stdio.h>
#include "GraphicData.h"
#include <string.h>

#define P_WIDTH 128
#define P_HEIGHT 64

#define ESC 0x1B

#define PIX_NONE 32
#define PIX_TOP 223
#define PIX_BOT 220
#define PIX_FULL 219

#define BLACK 0
#define RED 1
#define GREEN 2
#define BROWN 3
#define BLUE 4
#define PURPLE 5
#define GRAY 7
#define YELLOW 11
#define WHITE 15

#define BACKGROUND_COLOR GRAY

void print_byte(uint8_t byte);
void gotoxy(uint8_t x, uint8_t y);
void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
uint8_t getTopPaddedByte(const uint8_t* imageData, uint8_t byteIndex);
uint8_t getBottomPaddedByte(const uint8_t* imageData, uint8_t byteIndex);
uint8_t getByteAtPos(const uint8_t* imageData, uint8_t xPos, uint8_t yPos, uint8_t width);
void drawBgChar(uint8_t byte, uint8_t i);
void drawBgChar16(uint16_t byte, uint8_t i);
void drawBackground(const uint8_t* background);
void drawSpriteByte(uint8_t spriteByte, uint8_t bgByte, uint8_t spriteColor);
void drawSprite(const uint8_t* background, const uint8_t* spriteData, uint8_t spriteWidth, uint8_t spriteHeight, uint8_t spriteColor, uint8_t xPos, uint8_t yPos);
void resetGrid(uint8_t* grid);
void contaminateRect(uint8_t* grid, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void cleanRect(uint8_t* grid, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void drawCleanBackground(const uint8_t* background, uint8_t* cleanGrid);

#endif /* GRAPHICS_H_ */
