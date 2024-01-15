#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include <string.h>

void initLCD(void) {
	lcd_init();
}

void clearLCDBuffer(uint8_t buffer[]) {
	memset(buffer,0,512);
}

void drawLCD(uint8_t buffer[]) {
	lcd_push_buffer(buffer);
}

//Takes same translates from usual format
void addLCDbuffer(uint8_t* buffer,const uint8_t spirteData[], uint16_t spirteWidth, uint16_t spriteHeight, uint16_t x, uint16_t y) {
	uint16_t yCoord,xCoord;
	for (uint16_t i=0; i<spirteWidth;i++){
		for (uint16_t j=0; j<spriteHeight;j++){
			for(uint16_t k=0; k<8;k++){
				yCoord = (y+j*2+(k&0b1));
				xCoord = x+i*4+(yCoord>>3)*128+(k>>1);
				buffer[xCoord] &= ~(1<< (yCoord & 0x07));
				buffer[xCoord] |= (!!(spirteData[i+j*spirteWidth] & (1<<k)))<< (yCoord & 0x07);
			}
		}
	}
}
