#include "init.hpp"
#include "led-blaster-pre.hpp"
#include "config.h"

#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <pthread.h>
using namespace std;
//*********************************general INIT*******************************************
bool initGeneral(void)
{
	gpioTerminate(); //shut down all DMA channels and stuff so we can start fresh and easy ;-)

	if (gpioInitialise() < 0) //initializes the gpio libary
	{
	   cout << "pigpio initialisation failed." << endl;
	   return 1;
	}
	else
	{
    #ifdef DEBUG
	   cout << "pigpio initialised okay. initGeneral successful." << endl;
     #endif
	   return 0;
	}
}
