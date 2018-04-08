


#include "gpio.h"
#ifdef __cplusplus
extern "C"{
#endif
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio/tiny_gpio.h"
#include "Utils.h"
extern int errno;

void raiseOpenError(){
#ifdef __cplusplus
	extern "C" throw "Unable to open gpio register.";
#else
	fprintf(stderr,"Unable to open gpio register.");
	raise(SIGSEGV);
#endif
}

bool hasInit;

void init(){
	if(gpioInitialise()<0)
		raiseOpenError();
	hasInit = true;
}


void raiseIllegalPin(){
#ifdef __cplusplus
	extern "C" throw "Given Pin is Illegal";
#else
	fprintf(stderr,"Given Pin is Illegal, abborting");
	raise(SIGSEGV);
#endif
}

void connect(gpio target,mode m, int pin){
	if(!hasInit)init();
	target->pin = pin;
	target->d = m;
	gpioSetMode((unsigned)pin,(unsigned)m);
}
bool get(gpio target){
	if(target->d!=in)
		setMode(target,in);
	
	return gpioRead((unsigned)target->pin)==1?true:false;
}
void set(target pin,bool set){
	if(target->d!=out)
		setMode(target,out);
	if(set)
		gpioWrite((unsigned)target->pin,1);
	else
		gpioWrite((unsigned)target->pin,0);
}

mode getMode(gpio target){
	return target->d;
}
void setMode(gpio target,mode m){
	target->d = m;
	gpioSetMode((unsigned)target->pin,(unsigned)target->d);
}

bool waitOnEdge(gpio target){
	bool val = get(target);
	bool tmp = val;
	while((tmp = get(target))==val);
	val = tmp;
	return val;
}
uint64_t waitOnRisingEdge(gpio target){
	uint64_t t = getNanoseconds();
	if(waitOnEdge(target))
		return t-getNanoseconds();
	waitOnEdge(target);
	return t-getNanoseconds();
}
uint64_t waitOnFallingEdge(gpio target){
	int t = getNanoseconds();
	if(!waitOnEdge(target))
		return t-getNanoseconds();
	waitOnEdge(target);
	return t-getNanoseconds();
}
void connectSerial(serial target,mode m,gpio clock,gpio data,int clockPin,int dataPin){
	connect(clock,in,clockPin);
	connect(data,m,dataPin);
	target->clock = clock;
	target->data = data;
	target->clockCycles = -1;
}
void connectMasterSerial(serial target,mode m,gpio clock,gpio data,int clockPin,int dataPin,int clockCycles){
	connect(clock,out,clockPin);
	connect(data,m,dataPin);
	target->clock = clock;
	target->data = data;
	target->clockCycles = clockCycles;
}



void writeSerial(serial target,void* data,size_t size){
	int i;
	int j;
	char val;
	int time;
	if(target->clockCycles<0){
		//Slave
		for(i=0;i<size;i++){
			val = *((char*) data+i);
			for(j=0;j<8;j++){
				waitOnRisingEdge(target->clock);
				set(target->data,(val&(1<<j)!=0);
			}
			
		}
	}else{
		for(i=0;i<size;i++){
			val = *((char*)data+i);
			for(j=0;j<8;j++){
				time = target->clockCycles;
				set(target->clock,true);
				set(target->data,(val&(1<<j)!=0));
				EXACT_SLEEP(time)
				set(target->clock,false);
			}
				
		}
	}
	set(target->data,false);
}
void readSerial(serial target,void* readTo,size_t size){
	int i;
	int j;
	int time;
	if(target->clockCycles<0){
		//Slave
		for(i=0;i<size;i++){
			*((char*)readTo+i) = 0;
			for(j=0;j<8;j++){
				waitOnRisingEdge(target->clock);
				if(set(target->data))
					*((char*)readTo+i) |= 1<<j;
			}
			
		}
	}else{
		for(i=0;i<size;i++){
			*((char*)readTo+i) = 0;
			for(j=0;j<8;j++){
				time = target->clockCycles;
				set(target->clock,true);
				EXACT_SLEEP(time)
				if(set(target->data)
					*((char*)readTo+i) |= 1<<j;
				set(target->clock,false);
			}
				
		}
	}
}


#ifdef __cplusplus
};
#endif