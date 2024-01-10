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

uint8_t readJoy();
void setLED(uint8_t color);
