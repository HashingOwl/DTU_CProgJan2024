/*
 * io.h
 *
 *  Created on: Jan 5, 2024
 *      Author: rotho
 */

void setupJoystickPins();

void setPinToInput(GPIO_TypeDef *port, unsigned char pin);
uint16_t readPort(GPIO_TypeDef *port, uint16_t pins);

char readSinglePin(GPIO_TypeDef *port, uint8_t pin);


uint16_t readPin(GPIO_TypeDef *port, uint8_t pin);

void setPinInput(GPIO_TypeDef *port, uint8_t pin);

void setupLEDPins();

enum joy1Dir {
    joy1UP = 0b1,
    joy1DOWN = 0b10,
    joy1LEFT = 0b100,
    joy1RIGHT = 0b1000,
    joy1CENTER = 0b10000
};
uint8_t readJoy();
//void setLED(uint8_t color);
