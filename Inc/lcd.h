#ifndef LCD_H_
#define LCD_H_
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

void initLCD(void);
void clearLCDBuffer(uint8_t buffer[]);
void drawLCD(uint8_t buffer[]);
void addLCDbuffer(uint8_t buffer[],const uint8_t spirteData[], uint8_t spirteWidth, uint8_t spriteHeight, uint8_t x, uint8_t y);
void drawScore(uint8_t *slice, uint32_t score, uint8_t isHighScoreFlag);
void addLivesBuffer(uint8_t* buffer,uint8_t lives);
#endif /* LCD_H_ */
