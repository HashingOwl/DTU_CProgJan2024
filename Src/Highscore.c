#include "stm32f30x_conf.h" // STM32 config
#include "stdint.h"

#define baseAddress 0x0800F800
uint16_t readFlash(uint16_t offset) {
	return *(uint16_t *)(baseAddress + offset * 2); // Read Command
}

void writeFlash(uint16_t data[], uint16_t n) {
	FLASH_Unlock();
	FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
	FLASH_ErasePage(0x0800F800);
	for (uint16_t offset = 0; offset<n; offset++){
		FLASH_ProgramHalfWord(0x0800F800+offset*2, data[offset]);
	}
	FLASH_Lock();
}

//Return current highscore
uint32_t readHighscore(void) {
	uint32_t res = 0;
	res += readFlash(0)<<16;
	res += readFlash(1);
	return res;
}

//Checks if score is above old highscore and saves it if its the new hughscore
void saveHighscore(uint32_t score) {
	uint16_t newScore[2] = {0,0};
	if (score > readHighscore()) {
		newScore[0] = score >> 16;
		newScore[1] = score & 0x0000FFFF;
		writeFlash(newScore,2);
	}
}

//Resets the highscore
void resetScore(void) {
	uint16_t newScore[2] = {0,0};
	writeFlash(newScore,2);
}

