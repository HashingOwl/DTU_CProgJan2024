#include "graphics.h"
#include "graphicData.h"
#include "soundLib.h"

//Set true when you need to clean up visuals after returning from boss screen which is on an interrupt so it need to be global.
volatile uint8_t bossScreenReturnFlag = 0;

//This interrupt handles BossScreen.
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
	bossScreenReturnFlag = 1;
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
