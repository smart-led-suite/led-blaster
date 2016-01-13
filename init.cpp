#include "init.hpp"
#include "led-blaster-pre.hpp"
#include "config.h"

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <pthread.h>
using namespace std;
//*********************************general INIT*******************************************
bool initGeneral(void)
{
	std::cout << "init general..." << std::endl;
	return 0;
	/*
	gpioTerminate(); //shut down all DMA channels and stuff so we can start fresh and easy ;-)

	if (gpioInitialise() < 0) //initializes the gpio libary
	{
	   cout << "pigpio initialisation failed." << endl;
	   return 1;
	}
	else
	{
	   cout << "pigpio initialised okay. initGeneral successful." << endl;
	   return 0;
	}
	*/
}


//***************************PINS INIT******************************************************


bool initPin(int pin)
{

	std::cout << "init of pin " << pin << " successful." << std::endl;
	return 0;
}
