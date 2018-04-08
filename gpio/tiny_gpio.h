/*
	Header for tiny_gpio. Do not use, include the much more user friendly "gpio.h"
	This header exists for gpio.c
*/

void gpioSetMode(unsigned,unsigned);
unsigned gpioGetMode(unsigned);
int gpioRead(unsigned);
void gpioWrite(unsigned,unsigned);
int gpioInitialise();