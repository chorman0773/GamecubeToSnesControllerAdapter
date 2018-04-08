/*
	This file is part of the Gpio usuage library: Connor Horman, 2018
	
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
#ifndef __Utils_2018_04_08_00_08
#define __Utils_2018_04_08_00_08
#define EXACT_SLEEP(x) exactSleep(x)

#ifdef __cplusplus
extern "C"{
#endif
			
uint64_t getMicroseconds();
			
void exactSleep(uint64_t nanoseconds);

#ifdef __cplusplus
};
#endif
#endif