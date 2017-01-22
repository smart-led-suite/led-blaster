#include <iostream>
#include <stdint.h> //libary which includes uint16_t etc.
#include "config.h"
#include "led.hpp"
#include <pigpio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <pthread.h>
#include <time.h>       /* for the random function :time */
#include <cmath>

using namespace std;
using namespace led;
//init of static variables
int LED::pwmSteps = 1000;
int LED::fadeTime = 1000;
int LED::naturalSteps[1000];
std::map<int, led::LED*> LED::ledMap;
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
int LED::getTrueColorMultipier()
{
  return this->trueColorMultiplier;
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
void LED::setTrueColorMultipier(int new_Multiplier)
{
  trueColorMultiplier = 0;
  if (new_Multiplier > 0 && new_Multiplier < 100)
  {
    this->trueColorMultiplier = new_Multiplier;
  }
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
  if (new_cBrightness >= LED::getPwmSteps())
  {
    this->currentBrightness = LED::getPwmSteps();
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
  //std::cout << "new brightness: " << this->currentBrightness << std::endl;
  gpioPWM(this->pin, naturalSteps[this->currentBrightness]);
}
void LED::setTargetBrightness(int new_tBrightness)
{
  //new exception for blue, other treatment for blue colored strips as they are
  //brighter than other leds
  if (new_tBrightness > LED::getPwmSteps())
  {
    this->targetBrightness = LED::getPwmSteps();
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
  // generate natural steps array
  for (int inputval = 0; inputval <= 1000; inputval++) {
    naturalSteps[inputval] = ceil(0.001 * pow(inputval, 2));
    // std::cout << "curve values: " << inputval << " results in: " << naturalSteps[inputval] << '\n';
  }
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
  //INVALID CODE
}

//************************FADE***************************************************

//fade all leds off. called before terminating the programm
void LED::fadeAllLedsOff(void)
{
  for(auto const &iterator : LED::ledMap)
  {
    iterator.second->setTargetBrightness(0);
    iterator.second->fadeInThread();
  }
  for(auto const &iterator : LED::ledMap)
  {
    iterator.second->fadeWait();
  }
}
//turn all leds off instantly
void LED::turnAllLedsOff(void)
{
  for(auto const &iterator : LED::ledMap)
  {
    iterator.second->setCurrentBrightness(0);
  }

}

//***** THREAD EXPLANATION ************************
//for documentation about threads see:
//https://computing.llnl.gov/tutorials/pthreads/
//and about the pthread_create function see:
//https://computing.llnl.gov/tutorials/pthreads/man/pthread_create.txt
//************************************************
//fading in seperate threads allows us to make true simultaneous fading and handling each color individually
//the user can call the fadeCancel function itself, if he want's to start a new fading it is not necessary
//because every fade function checks if theres a fade going on at that moment
void LED::fadeInThread(void)
{
    fadeCancel();
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
    printf("joining thread\n");
    int err = pthread_join(this->fadeThread, NULL);
    printf("thread joined\n");
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
  //stepsize for the fade
  int stepsize = 1;
  //calculate the real target brightness it will have (after the trueColorAdjust)
  int trueTargetBrightness;
  if (trueColorMultiplier > 0 && trueColorMultiplier < 100 && this->targetBrightness > 0)
  {
    //lower the brightness of the color by a factor (0 = 0%, at 100% [not allowed] there wouldnt be any light)
    trueTargetBrightness = this->targetBrightness - ((this->targetBrightness * trueColorMultiplier) / 100);
  }
  else
  {
    trueTargetBrightness = this->targetBrightness;
  }
  //std::cout << "true brightness: " << trueTargetBrightness << std::endl;
  //calculate the delayUs needed to archieve the specified fadeTime
  //steps * delayUs * 1000 = fadeTime [in ms]
  //fadeTime is the variable which sets the Time needed to fade

  int totalSteps = ((trueTargetBrightness) - (this->currentBrightness));
  //we want to have a positive steps number
  if (totalSteps < 0)
  {
    totalSteps = -totalSteps;
  }
  if (totalSteps > 0)
  {
    //calculate delay in us
    int delayUs = ((LED::fadeTime) * 1000 / (totalSteps)) / stepsize;
    //to avoid flickering we'll increase the steosize if the delay is smaller than 6us #
    //(as the pwm duty time gets updated every 5us at 200Hz)
    while (delayUs <= 15)
    {
      stepsize++;
      delayUs = ((LED::fadeTime) * 1000 / (totalSteps)) / stepsize; //calculate delay in us
    }
    #ifdef DEBUG
      uint32_t startTime = 0; //time to check if there's any overhead
      uint32_t endTime = 0; //time to check if there's any overhead
      startTime = gpioTick();
    #endif
    //fading to target brightness
  	for (int current = this->currentBrightness; current <= trueTargetBrightness; current = current + stepsize)
  	{
  				//increase currentBRIGHTNESS of that color and write it to the pin
  				this->setCurrentBrightness(current); //we use +1 so it will actually reach the targetBrightness
          gpioDelay(delayUs); 		//make a delay
  	}
    for (int current = this->currentBrightness; current >= trueTargetBrightness; current = current - stepsize)
    {
          //decrease currentBRIGHTNESS of that color and write it to the pin
          this->setCurrentBrightness(current); //we use -1 so it will actually reach the targetBrightness
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
    fadeCancel();
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


LED::LED(std::string led_colorcode, uint16_t led_pin, bool led_isColor, int led_currentBrightness, int led_targetBrightness, int led_trueColorMultiplier)
{
  this->colorcode = led_colorcode;
  this->pin = led_pin;
  this->isColor = led_isColor;
  this->currentBrightness = led_currentBrightness;
  this->targetBrightness = led_targetBrightness;
  this->fading = false;
  this->randomlyFading = false;
  setTrueColorMultipier(led_trueColorMultiplier);
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
