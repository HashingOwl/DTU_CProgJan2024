#ifndef _Joystick_H_
#define _Joystick_H_
/* Includes -------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f30x_conf.h"
// Whatever needs to be included
/* Exported types -------------------------------------------------------*/
/* Exported constants ---------------------------------------------------*/
/* Exported macro -------------------------------------------------------*/

 // Whatever needs to be defined
/* Exported functions -------------------------------------------------- */
void initAnalogJoystick();
uint8_t readJoystickButtons(void);
uint8_t readAnalogJoystickDigital();
void readAnalogJoystick(uint32_t* x, uint32_t* y);
#endif /* _ANSI_H_ */

