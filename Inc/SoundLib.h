/*
 * SoundLib.h
 *
 *  Created on: 13. jan. 2024
 *      Author: alber
 */

#ifndef SOUNDLIB_H_
#define SOUNDLIB_H_

/* Includes -------------------------------------------------------------*/
#include <stdint.h>// Whatever needs to be included
/* Exported types -------------------------------------------------------*/
/* Exported constants ---------------------------------------------------*/
/* Exported macro -------------------------------------------------------*/

 // Whatever needs to be defined
/* Exported functions -------------------------------------------------- */
//New system
/*
void soundInit(void);
void TIM2_IRQHandler(void);
void TIM1_UP_TIM16_IRQHandler(void);
*/

//old system

void soundInit(void);
void TIM2_IRQHandler(void);
void TIM1_UP_TIM16_IRQHandler(void);
void setFreqARR(uint16_t ARRval);
void updateMusic();
void disableMusic();
extern volatile uint8_t musicUpdateFlag;

#endif
