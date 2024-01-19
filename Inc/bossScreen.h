/*
 * bossScreen.h
 *
 *  Created on: 19. jan. 2024
 *      Author: alber
 */

#ifndef BOSSSCREEN_H_
#define BOSSSCREEN_H_
//Set true when you need to clean up visuals after returning from boss screen which is on an interrupt so it need to be global.
extern volatile uint8_t bossScreenReturnFlag;

//This interrupt handles BossScreen.
void EXTI4_IRQHandler(void);

void initBossScreen(void);

#endif /* BOSSSCREEN_H_ */
