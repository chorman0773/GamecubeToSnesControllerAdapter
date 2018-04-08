#ifndef __gpio_h_2018_03_06_21_30
#define __gpio_h_2018_03_06_21_30
#pragma once
#ifdef __cplusplus
extern "C"{
#endif
#include <stdint.h>
#include <stdbool.h>
typedef enum{
	in, out
}mode;

struct _gpio{
	mode d;
	unsigned pin;
};
struct _serial{
	struct _gpio* clock;
	struct _gpio* data;
	int clockCycles;
};

typedef _gpio* gpio;
typedef _serial* serial;

void connect(gpio,mode,int);

bool get(gpio);
void set(gpio,bool);
mode getMode(gpio);
void setMode(gpio);


bool waitOnEdge(gpio);
uint64_t waitOnRisingEdge(gpio);
uint64_t waitOnFallingEdge(gpio);




void connectSerial(serial,mode,gpio,gpio,int,int);
void connectMasterSerial(serial,mode,gpio,gpio,int,int,int);

void writeSerial(serial,void*,size_t);
void readSerial(serial,void*,size_t);


#ifdef __cplusplus
};
#endif

#endif