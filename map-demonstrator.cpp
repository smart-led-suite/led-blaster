//============================================================================
// Name        : hello.cpp
// Author      : Sandesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
// ToDo	       : 
//	(done)		 - new universal initPin
//	(done)		 - introduce header file so the functions can be placed below the main part
//	(done)		 - introduce file to save current luminances
//	(done)		 - introduce simultaneous fadeAlgorithm
//			 - introduce separate files
//			 - improve simultaneous algorithm
//			 - move currentBrightness read/write to own function
//			 - introduce other fadeModes (aka exp fade)
//	(done, untested) - introduce pipes (major)
//		 	 - introduce some other modes (fade w/ overlap of different colors)
//			 - introduce music mode
//			 - introduce config file w/ led pins etc.
//			 - tidying up code changes due to implementation of FIFO (BLOCKER)
//============================================================================

#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>

#include <string>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>

// MOVE #defines into header
#define FIFO_FILE	"/dev/led-blaster"

using namespace std;

char wname[] = "white";
char rname[] = "r";
char gname[] = "g";
char bname[] = "b";

int wval = 1000;
int rval = 200;
int gval = 500;
int bval = 0;

map < std::string, int> pin = 
	{
		{"white", 25}, 
		{"r", 17},
		{"g", 18},
		{"b", 22}
	};
	map < string, int> ledsTarget = 
	{
		{"white", 0}, 
		{"r", 1000},
		{"g", 250},
		{"b", 500}
	};
	map < string, int> ledsCurrent = 
	{
		{"white", 500}, 
		{"r", 250},
		{"g", 400},
		{"b", 100}
	};

int main(int argc, char* argv[]) {
	
	
	//check variables:
	
	for(auto const &colors : pin)
	{
  		// ent1.first is the first key
  		cout << "color and pin seperated by space: " << colors.first << " " << colors.second << endl;
  		//now search the target brightness for current color (colors.first)
  		auto targetBrightness = ledsTarget.find(colors.first);
  		if (targetBrightness != ledsTarget.end())
  		{
  			cout << "target brightness: " << targetBrightness->second << endl;
  		}	
  		auto currentBrightness = ledsCurrent.find(colors.first);
  		if (currentBrightness != ledsCurrent.end())
  		{
  			cout << "current brightness: " << currentBrightness->second << endl;
  		}
  		
	}
	
	//simulate interactive input
	ledsTarget[wname] = wval;
	ledsTarget[rname] = rval;
	ledsTarget[gname] = gval;
	ledsTarget[bname] = bval;
	
	//check variables again
	
	for(auto const &colors : pin)
	{
  		// ent1.first is the first key
  		cout << "color and pin seperated by space: " << colors.first << " " << colors.second << endl;
  		//now search the target brightness for current color (colors.first)
  		auto targetBrightness = ledsTarget.find(colors.first);
  		if (targetBrightness != ledsTarget.end())
  		{
  			cout << "target brightness: " << targetBrightness->second << endl;
  		}	
  		auto currentBrightness = ledsCurrent.find(colors.first);
  		if (currentBrightness != ledsCurrent.end())
  		{
  			cout << "current brightness: " << currentBrightness->second << endl;
  		}
  		
	}

	
}

