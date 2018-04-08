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
#ifndef __SNES_Controller_cpp_2018_03_07_08_49
#define __SNES_Controller_cpp_2018_03_07_08_49
#include "gpio.h"

#define BUTTON_A 0x100
#define BUTTON_B 0x01
#define BUTTON_X 0x200
#define BUTTON_Y 0x02
#define DPAD_U 0x10
#define DPAD_D 0x20
#define DPAD_L 0x40
#define DPAD_R 0x80
#define BUTTON_L 0x400
#define BUTTON_R 0x800
#define BUTTON_START 0x08
#define BUTTON_SELECT 0x04
#define NUMBER_SNES 12


class SNESController{
private:
	int ci_ExportButtons;
	int currButtons;
	gpio dataLatch;
	gpio serialClock;
	gpio serialData;
	serial export_Data;
	
public:
	SNESController(int clock,int latch,int data);
	~SNESController();
	void setButtons(int buttons);
	void clearButtons(int buttons);
	void volatile_exportButtons();
	void forceLegalInputs();
};





#endif