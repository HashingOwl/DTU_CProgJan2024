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
void initJoystickAnalog ();
void readJoystickAnalog();
uint8_t readJoystickDigital();
#endif /* _ANSI_H_ */

