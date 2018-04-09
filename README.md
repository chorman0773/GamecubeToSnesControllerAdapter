    Gamecube Controller to SNES Adapater, Connor Horman 2018
    This program is intended to be compiled on a Raspberry Pi, using GCC and G++. To compile run g++ -o <Output File> SNES-Controller.cpp GCController.cpp SNESToGCAdpater.cpp & gcc gpio.c gpio/tinyGpio.c
    
    This program allows you to use a raspberry pi to adapt Input from a Gamecube Controller to a SNES Controller output.
    The program can support up to 2 inputs and 2 outputs, both input ports must be provided by you and attached to the GPIO pins yourself.
    This program supports Source Configuration and (TODO) Dynamic Configuration via Inet.
    (Dynamic Configuration is planned but not implemented yet, if you wish to implement it using Linux's Socket IO you may do so.)
    
	
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    Credits:
    The tinyGpio library wrapped by gpio does not belong to me and was released to public domain by the owner.
    If you are the owner of the tinyGpio library, I can add your name to the credits if you wish.
