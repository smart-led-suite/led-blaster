#include <iostream>
//#include <map>
#include <stdint.h> //libary which includes uint16_t etc.
#include "config.h"
#include "led.hpp"
//#include "init.hpp"

//#include <stdio.h>
#include <pigpio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <pthread.h>
#include <time.h>       /* for the random function :time */


using namespace std;

//init of static variables
int LED::pwmSteps = 1000;
int LED::fadeTime = 1000;
//********++getter*****************
std::string LED::getColorCode()
{
  return this->colorcode;
}
uint16_t LED::getPin()
{
  return this->pin;
}
bool LED::IsColor()
{
  return this->isColor;
}
bool LED::isFading()
{
  return this->fading;
}
bool LED::isRandomlyFading()
{
  return this->randomlyFading;
}
int LED::getCurrentBrightness()
{
  return this->currentBrightness;
}
int LED::getTargetBrightness()
{
  return this->targetBrightness;
}
int LED::getPwmSteps()
{
  return LED::pwmSteps;
}
pthread_t LED::getFadeThread()
{
  return this->fadeThread;
}
int LED::getFadeTime(void)
{
  return LED::fadeTime;
}
//************************setter************************************************
void LED::setFadeTime(int newFadeTime)
{
  //we won't accept smaller numbers because we dont want to waste too much cpu power
  if (newFadeTime > 0)
  {
    LED::fadeTime = newFadeTime;
  }
  else
  {
    std::cerr << "please set a fadetime greater than zero" << std::endl;
  }
}
void LED::setColorCode(std::string new_colorcode)
{
  this->colorcode = new_colorcode;
}
void LED::setPin(uint16_t newpin)
{
  this->pin = newpin;
}
void LED::setIsColor(bool newisColor)
{
  this->isColor = newisColor;
}
void LED::setCurrentBrightness(int new_cBrightness)
{
  if (new_cBrightness >= LED::pwmSteps)
  {
    this->currentBrightness = LED::pwmSteps;
  }
  else if (new_cBrightness < 0)
  {
    this->currentBrightness = 0;
  }
  else
  {
  this->currentBrightness = new_cBrightness;
  }
  //set pin to the new brightness
  gpioPWM(this->pin, this->currentBrightness);
}
void LED::setTargetBrightness(int new_tBrightness)
{
  if (new_tBrightness >= LED::pwmSteps)
  {
    this->targetBrightness = LED::pwmSteps;
  }
  else if (new_tBrightness < 0)
  {
    this->targetBrightness = 0;
  }
  else
  {
  this->targetBrightness = new_tBrightness;
  }
}

//************************FUNCTIONS*************************************************+

//*********************************general INIT*******************************************
bool LED::initGeneral(void)
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

//************************FADE***************************************************
//***** THREAD EXPLANATION ************************
//for documentation about threads see:
//https://computing.llnl.gov/tutorials/pthreads/
//and about the pthread_create function see:
//https://computing.llnl.gov/tutorials/pthreads/man/pthread_create.txt

void LED::fadeInThread(void)
{
  if (this->fading)
  {
    fadeCancel();
  }
  else
  {
    this->fading = true;
    LED *led = this;
    //fadeLauncher: starts and stops thread and so on
    int err = pthread_create(&(this->fadeThread), NULL, fadeLauncher, (void *) led);
    if(err != 0)
    {
      std::cerr << "creating fadeThread not possible. errorcode: " << err << std::endl;
      exit(1);
    }
  }
  //delete fadeStruct;
}

//as pthread_create doesn't support non-static methods we'll need a workaround
//found here: http://stackoverflow.com/questions/1151582/pthread-function-from-a-class
void * LED::fadeLauncher(void *context)
{
  //convert the context pointer into a struct pointer to get the fadeTime
  //then convert it back into a void pointer
  //call fade() and return the value to pthread
  void * returnValue = ((LED *)context)->fade();
  ((LED *)context)->fading = false;
  pthread_exit(NULL);
  return returnValue;
}

void LED::fadeCancel(void)
{
  if(this->fading || this->randomlyFading)
  {
    pthread_cancel(this->fadeThread);
    this->fading = false;
    this->randomlyFading = false;
  }
}

void LED::fadeWait(void)
{
  if (this->randomlyFading == false)
  {
    int err = pthread_join(this->fadeThread, NULL);
    if (err)
    {
      std::cerr << "error while joining thread. errno: " << err << std::endl;
    }
    else
    {
      this->fading = false;
    }
  }
  else
  {
    //if we're randomly fading we'd have to wait infinitely
    this->fadeCancel();
    this->randomlyFading = false;
  }
}

void * LED::fade(void)
{
  //this->fading = true;
  //#define DEBUG
  //calculate the delayUs needed to archieve the specified fadeTime
  //steps * delayUs * 1000 = fadeTime [in ms]
  //fadeTime is the variable which sets the Time needed to fade
  int totalSteps = ((this->targetBrightness) - (this->currentBrightness));
  //we want to have a positive steps number
  if (totalSteps < 0)
  {
    totalSteps = -totalSteps;
  }
  if (totalSteps > 0)
  {
    int delayUs = (LED::fadeTime) * 1000 / (totalSteps); //calculate delay in us
    #ifdef DEBUG
      uint32_t startTime = 0; //time to check if there's any overhead
      uint32_t endTime = 0; //time to check if there's any overhead
      startTime = gpioTick();
    #endif
    //fading to target brightness
  	for (int current = this->currentBrightness; current < this->targetBrightness; current++)
  	{
  				//increase currentBRIGHTNESS of that color and write it to the pin
  				this->setCurrentBrightness(current + 1); //we use +1 so it will actually reach the targetBrightness
          gpioDelay(delayUs); 		//make a delay
  	}
    for (int current = this->currentBrightness; current > this->targetBrightness; current--)
    {
          //decrease currentBRIGHTNESS of that color and write it to the pin
          this->setCurrentBrightness(current - 1); //we use -1 so it will actually reach the targetBrightness
          gpioDelay(delayUs); 		//make a delay
    }

  	#ifdef DEBUG
      endTime = gpioTick(); //time needed for the fade
  		printf("time variable for fade: %d \n", LED::fadeTime);
      //end-start gives elapsed time in micros; divided by 1000 we have it in millis to compare
  		cout << "real time needed for fade: " << ((endTime - startTime) / 1000) << endl;
  	#endif
  }
  //this->fading = false;
  return 0;
}

//****random fade*****
void LED::fadeRandomInThread(void)
{
  if (this->fading || this->randomlyFading)
  {
    fadeCancel();
  }
  else
  {
    this->fading = true;
    this->randomlyFading = true;
    LED *led = this;
    //fadeLauncher: starts and stops thread and so on
    int err = pthread_create(&(this->fadeThread), NULL, fadeRandom, (void *) led);
    if(err != 0)
    {
      std::cerr << "creating fadeRandomThread not possible. errorcode: " << err << std::endl;
      exit(1);
    }
  }
  //delete fadeStruct;
}

//as pthread_create doesn't support non-static methods we'll need a workaround
//found here: http://stackoverflow.com/questions/1151582/pthread-function-from-a-class
void * LED::fadeRandom(void *context)
{
  //instead of this-> we'll use ((LED *)context)
  ((LED *)context)->fading = true;
  ((LED *)context)->randomlyFading = true;
  /* initialize random seed: */
  srand (time(NULL));
  while (true)
  {
		//rand % 1000 creates numbers between 0 and 999, ...
		//% 1001 should create nubers between 0 and 1000
		((LED *)context)->setTargetBrightness(rand() % 1001);
    //start fading (fade() is a blocking fade)
    ((LED *)context)->fade();
  }
  //INVALID CODE
}

//***************************PINS INIT******************************************************
int LED::initPin(void)
{
  int PWMrange = 0; //variable to check the pwm range against the realpwm range
  int pwmSteps;
	#if PWM_CONFIG
		int realRange = 0;
		int setRange = 0;
		int targetRange = 0;

		int targetFrequency = 0;
		int frequency = 0;

		cout << "enter target frequency :" << endl;
		cin >> targetFrequency;
		cout << "set frequency to: " << targetFrequency << endl;
		frequency = gpioSetPWMfrequency(pin, targetFrequency);
		cout << "range was set to closest frequency: " << frequency << endl;
		cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;

		cout << "enter target range :" << endl;
		cin >> targetRange;
		cout << "set range to: " << targetRange << endl;
		realRange = gpioSetPWMrange(pin, targetRange);
		cout << "range was set to real range: " << realRange << endl;
		cout << "check range: " << gpioGetPWMrange(pin) << endl;
	#endif


	if (gpioSetMode(this->pin, 1)) //set pin to output (1), returns 0 if it went ok
	{
		cout << "error while setting " << this->pin << " to output" << endl;
		return 0;
	}

	/**************INFOS ABOUT PWM FREQUENCY AND RANGE*******************************
	some infos for setting the pwm range of each pin
	a sample rate of 5 (default) means that one step can be at least 5us "long"
	so if you use a frequency of 200Hz there will be 1000 real steps
	Calculations:
		1/200s is one cycle = 0,005s which is 5ms (Millis)
		1ms equals 1000us (Microseconeds)
		so within 5ms there are 1000 5us steps possible
		-> if you increase the frequency you have to decrease the range
	as far as I understand its realized like that in the pigpio libary:
	you set the frequency to whatever you like (w/ SetPWMFrequency)
	you set the pwm_range to whatever you like (w/ SetPWMRange)
	now you can check the pwmSteps to see how many steps you actually have
	(i.e. if you set your frequency to 200Hz and the range to 5000 this will
	return 1000 (see calculations above))
	BUT if I understand it correctly it uses the range you set (5000 in this example)
	so 5000 is the maximum brightness BUT between 0 and 4 there's no real step!
	this is the difference between the realRange and the range. In my opinion
	it's good if both have the same value (which is true for 200Hz and 1000steps)
	********************************************************************************
	*/

	#ifdef DETAILED_PIN_INIT_INFORMATION
		cout << "set " << this->pin << " to output" << endl;
	#endif
	gpioSetPWMfrequency(this->pin, PWM_FREQUENCY);
	#ifdef DETAILED_PIN_INIT_INFORMATION
		cout << "check frequency: " << gpioGetPWMfrequency(this->pin) << endl;
	#endif
	gpioSetPWMrange(this->pin, PWM_RANGE);
	pwmSteps = gpioGetPWMrealRange(this->pin);
	#ifdef DETAILED_PIN_INIT_INFORMATION
		cout << "check real pwm range: " << pwmSteps << endl;
	#endif
	PWMrange = gpioGetPWMrange(this->pin);
	#ifdef DETAILED_PIN_INIT_INFORMATION
		cout << "check pwm range: " << PWMrange << endl;
	#endif
  if (PWMrange != pwmSteps)
  {
    std::cout << "warning: set pwm range and real pwm range don't match." << std::endl;
    std::cout << "pwmrange: " << PWMrange << " pwmSteps: " << pwmSteps << std::endl;
  }

	gpioPWM(this->pin, 0); //set brightness to 0
	//cout << "pwm set to 0 correctly" << endl;
  #ifdef DEBUG
	cout << "init of pin " << this->pin << " finished successfully." << endl;
  #endif
	return pwmSteps;
}


LED::LED(std::string led_colorcode, uint16_t led_pin, bool led_isColor, int led_currentBrightness, int led_targetBrightness)
{
  this->colorcode = led_colorcode;
  this->pin = led_pin;
  this->isColor = led_isColor;
  this->currentBrightness = led_currentBrightness;
  this->targetBrightness = led_targetBrightness;
  this->fading = false;
  this->randomlyFading = false;
  //initializes each pin. returns 0 if everything went ok
  //std::cout << "initializes color  \"" << colorcode << "\"" << std::endl;
  //pwmSteps is static and should have the same value for every pin
  LED::pwmSteps = LED::initPin();
  if(LED::pwmSteps == 0) {
    //print that there has been an error if this happens (very unlikely)
    cout << "error in initPin " << pin;
    cout << "which is used by color " << colorcode << endl;
    //and exit the program
    exit(EXIT_FAILURE);
  }
}

LED::~LED(void)
{
  //std::cout << "destroy led object" << std::endl;
  //this->setCurrentBrightness(0);
}
