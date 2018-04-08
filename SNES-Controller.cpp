
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

#include "SNES-Controller.hpp"

#define HANDLE_UD 0x10
#define MASK_UD 0x30
#define HANDLE_LR 0x40
#define MASK_LR 0xc0

#define MASK_HEAD 0xf000
#define BUTTON_MASK 0x0fff


SNESController::SNESController(int clock,int latch,int data){
	this->dataLatch = new _gpio;
	this->serialClock = new _gpio;
	this->serialData = new _gpio;
	this->export_Data = new _serial;
	connect(this->dataLatch,in,latch);
	connectSerial(this->export_Data,out,this->serialClock,this->serialData,clock,data);
	this->currButtons = MASK_HEAD;
	this->ci_ExportButtons = MASK_HEAD;
}
void SNESController::setButtons(int buttons){
	this->currButtons = MASK_HEAD|(buttons&BUTTON_MASK);
}
void SNESController::clearButtons(int buttons){
	this->currButtons = MASK_HEAD|((~buttons)&BUTTON_MASK);
}
void SNESController::forceLegalButtons(){
	this->ci_ExportButtons = MASK_HEAD| (this->ci_ExportButtons&BUTTON_MASK);
	if((this->ci_ExportButtons&MASK_UD)==MASK_UD)
		this->ci_ExportButtons = MASK_HEAD| ((this->ci_ExportButtons&~MASK_UD)|HANDLE_UD)&BUTTON_MASK;
	if((this->ci_ExportButtons&MASK_LR)==MASK_LR)
		this->ci_ExportButtons = MASK_HEAD| ((this->ci_ExportButtons&~MASK_LR)|HANDLE_LR)&BUTTON_MASK;
}

void SNESController::volatile_ExportButtons(){
	waitOnRisingEdge(this->dataLatch);
	this->ci_ExportButtons = this->currButtons;//Latch buttons on the rising edge of the dataLatch
	forceLegalButtons();
	waitOnFallingEdge(this->dataLatch);
	writeSerial(this->export_Data,&this->ci_ExportButtons,2);
}

SNESController::~SNESController(){
	delete this->dataLatch;
	delete this->serialClock;
	delete this->serialData;
	delete this->export_Data;
}
