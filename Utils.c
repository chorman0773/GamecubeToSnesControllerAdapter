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
#include "Utils.h"

uint64_t getNanoseconds(){
	struct timespec time;
	clock_gettime(CLOCK_REALTIME,&time);
	return time.tv_sec*(uint64_t)1000000000+time.tv_nsec;
}

void exactSleep(uint64_t nanoseconds){
	struct timespec time;
	clock_gettime(CLOCK_REALTIME,&time);
	time.tv_sec += nanoseconds/(uint64_t)1000000000;
	time.tv_nsec += nanoseconds%(uint64_t)1000000000;
	clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&time,NULL);
}