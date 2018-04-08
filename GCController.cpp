/*
	This file is a part of: Gamecube Controller to SNES Adapater by Connor Horman 2018
	
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "GCController.hpp"
#include "Utils.h"
extern int axisDeadZone;
void GCPort::sendSignal(bool val){
	while(this->isReading);
	this->isWriting = true;
	set(this->data,false);
	if(val)EXACT_SLEEP(1000);
	else EXACT_SLEEP(3000);
	set(this->data,true);
	this->isWriting = false;
}

bool GCPort::readSignal(){
	uint64_t time;
	while(this->isWriting);
	this->isReading = true;
	waitOnFallingEdge();
	time = waitOnRisingEdge()/1000;
	this->isReading = false;
	if(time>=2)
		return false;
	return true;
}
void GCPort::sendData(void* data,int bits){
	char* c = (char*)data;
	int bytes = bits/8;
	for(int i = bytes-1;i>=0;i--)
		for(int j = 0;j<(bits/(bytes-i))%8;j++)
			sendSignal((c[i])>>(8-j))&1);
	sendSignal(true);
}
void GCPort::readData(void* read,int bits){
	char* c = (char*)data;
	char currByte = 0;
	int bytes = bits/8;
	for(int i = bytes-1;i>=0;i--){
		currByte = 0;
		for(int j = 0;j<(bits/(bytes-i))%8;j++){
			currByte<<1;
			currByte |= readSignal()?1:0;
		}
		c[i] = currByte;
	}
	readSignal();//Read null
}

void GCPort::poll(){
	uint64_t msg;
	if(!active)
	{
		msg = 0;
		sendData(&msg,8);
	}else
	{
		msg = this->rumbleOn?0x400303:0x400302;
		sendData(&msg,24);
		readData(&msg,64);
		this->buttons = msg&0xFFFF;
		this->axisXYZrZ = (msg>>16)&0xFFFFFFFF;
	}
	
}
bool GCPort::isButtonSet(int button){
	if(!active)return false;
	return (this->buttons&((1<<button)-1))!=0;
}
bool GCPort::isXNotDead(){
	isAxisNotDead(GC_AXIS_X);
}

bool GCPort::isYNotDead(){
	return isAxisNotDead(GC_AXIS_Y);
}

bool GCPort::isZNotDead(){
	return isAxisNotDead(GC_AXIS_Z);
}
bool GCPort::isrZNotDead(){
	return isAxisNotDead(GC_AXIS_rZ);
}
bool GCPort::isAxisNotDead(int axis){
	if(!acitve)return false;
	int val = (this->axisXYZrZ>>(axis*8))&0xFF - 0x80;
	return val>axisDeadZone||val<-axisDeadZone;
}
int GCPort::getActivation(int axis){
	if(!isAxisNotDead(axis))return 0;
	else if((signed char)(this->axisXYZrZ>>(axis*8) - 0x80)<0)
		return -1;
	else
		return 1;
}

void GCPort::rumble(){
	this->rumbleOn = true;
}
void GCPort::stopRumble(){
	this->rumbleOn = true;
}

GCPort::GCPort(){
	data = new _gpio;
	this->active = false;
	this->rumbleOn = false;
	this->buttons = 0;
	this->axisXYZrZ = 0;
	this->isReading = false;
	this->isWriting = true;
}

GCPort::~GCPort(){
	delete data;
}

GCPort::checkAttached(){
	int _;
	if(!this->active)
		this->readData(&_,8);
	this->active = true;
}

