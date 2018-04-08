
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

	Full Program to Take Inputs from a GC Controller and Output to a snes port. Reverse may be coming soon...
	Current Setup: (Physical Numbering)
	1 -> GC Ports, 3.3V Source
	3 -> GC Port 1, Data Line
	4 -> GC Ports, 5V Source
	5 -> GC Port 2, Data Line
	6 -> GC Ports, GND
	
*/

#include "SNES-Controller.hpp"
#include "GCController.hpp"
#include "SNESToGC.hpp"
#include "Utils.h"
#include <thread>
#include <chrono>

#define GC1_DATA 2

#define GC2_DATA 3


#define SNES1_CLOCK 14
#define SNES1_LATCH 15
#define SNES1_DATA  18

#define SNES2_CLOCK 17
#define SNES2_LATCH 27
#define SNES2_DATA  22

using std::thread;
using std::this_thread;
using std::chrono;

const char* const snesButtonNames[] = {    "B",   "Y","Select","Start","Up","Down","Left","Right","A","X","L","R"};
const char* const gcButtonNames[] =   {"Left","Right",  "Down",   "Up", "Z",   "R",  "L","","A","B","X","Y","Start"};
const char* const gcAxisNames[] = {"Analog X","Analog Y","C-Stick X","C-Stick Y"};
int axisControlledButtons[][] =       {{DPAD_L,DPAD_R},
									   {DPAD_U,DPAD_D},
									   {     0,     0},
									   {     0,     0}};
int gcButtonMap[] =       {DPAD_L,        DPAD_R,
						   DPAD_D,        DPAD_U,
						   BUTTON_X,BUTTON_R,
						   BUTTON_L,           -1,
						   BUTTON_A,     BUTTON_B,
						   BUTTON_SELECT,     BUTTON_Y,
						   BUTTON_START          };

extern int axisDeadZone = 8;

GCPort*           gc1;
GCPort*           gc2;
SNESController* snes1;
SNESController* snes2;

void setAxisDeadZone(int deadZone){
	axisDeadZone = deadZone;
}

void mapAxis(int gcAxis,int buttonPositive,int buttonNegative){
	axisControlledButtons[gcAxis][0] = buttonPositive;
	axisControlledButtons[gcAxis][1] = buttonNegative;
}
void mapAxisPositive(int gcAxis,int snesButton){
	axisControlledButtons[gcAxis][0] = snesButton;
}
void mapAxisNegative(int gcAxis,int snesButton){
	axisControlledButtons[gcAxis][1] = snesButton;
}

void mapButton(int gcButton,int snesButton){
	gcButtonMap[gcButton] = snesButton;
}

void exportToSNES(){
	int buttons = 0;
	for(int i = 0;i<NUMBER_GC_AXIS;i++)
		if(gc->isAxisNotDead(i))
			buttons |= axisControlledButtons[i][gc->getActivation(i)<0?1:0];

	for(int i = 0;i<NUMBER_GC;i++)
		if(gc->isButtonSet(i))
			buttons |= gcButtonMap[i];
	snes->setButtons(buttons);
	snes->clearButtons(~buttons);
}


bool hasStarted = false;
bool shouldExit = false;

void exec_daemon_ReadButtons(){
	while(!hasStarted);
	while(!shouldExit){
		gc->poll();	
		this_thread::sleepFor(chrono::miliseconds(12));
	}
}
void exec_daemon_WriteSnes(){
	while(!hasStarted);
	while(!shouldExit)
		snes->volatile_exportButtons();
	
}
void exec_daemon_Export(){
	while(!hasStarted);
	while(!shouldExit){
		exportToSNES();
		this_thread::sleepFor(chrono::miliseconds(1000/120));
	}
}

thread daemon_ReadButtons(exec_daemon_ReadButtons());
thread daemon_WriteSnes(exec_daemon_WriteSnes());
thread daemon_Export(exec_daemon_Export());

int main(){
	gc1 = new GCPort(2);
	gc2 = new GCPort(3);
	snes1 = new SNESController(14,15,18);
	snes2 = new SNESController(17,27,22);
	hasStarted = true;
	while(!shouldExit);
	delete gc1;
	delete gc2;
	delete snes1;
	delete snes2;
}



