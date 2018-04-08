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
#ifndef __GCController_hpp_2018_03_07_09_21
#define __GCController_hpp_2018_03_07_09_21

#define GC_DPAD_RIGHT   0
#define GC_DPAD_LEFT    1
#define GC_DPAD_DOWN    2
#define GC_DPAD_UP      3
#define GC_BUTTON_Z     4
#define GC_BUTTON_R     5
#define GC_BUTTON_L     7
#define GC_BUTTON_A     8
#define GC_BUTTON_B     9
#define GC_BUTTON_X     10
#define GC_BUTTON_Y     11
#define GC_BUTTON_START 12
#define NUMBER_GC       13

#define GC_AXIS_X       0
#define GC_AXIS_Y       1
#define GC_AXIS_Z       2
#define GC_AXIS_rZ      3
#define NUMBER_GC_AXIS  4

class GCPort{
private:
	int axisXYZrZ;
	gpio data;
	int buttons;
	bool active;
	bool rumbleOn;
	bool isReading;
	bool isWriting;
	thread checkAttached_daemon;
	void sendSignal(bool);
	bool readSignal();
	void sendData(void*,int bits);
	void readData(void*,int bits);
public:
	GCPort(int dataPort);
	~GCPort();
	bool isButtonSet(int button);
	bool isXNotDead();
	bool isYNotDead();
	bool isZNotDead();
	bool isrZNotDead();
	bool isAxisNotDead(int axis);
	int getActivation(int axis);
	void rumble();
	void stopRumble();
	void checkForAttached();
	void poll();
};


#endif