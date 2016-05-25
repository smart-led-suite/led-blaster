
#include "fade.hpp"
#include "led.hpp"
#include "led-blaster-pre.hpp"
#include "config.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
#include <utility>

void turnLedsOff(ledInformationStruct * led)
{
  LED::setFadeTime(led->fadeTime);
	//set targetBrightness of all colors to 0
	for (size_t ledsAvailable = 0; ledsAvailable < led->leds.size(); ledsAvailable++)
	{
		led->leds[ledsAvailable].setTargetBrightness(0);
    led->leds[ledsAvailable].fadeInThread();
	}
  for (size_t ledsAvailable = 0; ledsAvailable < led->leds.size(); ledsAvailable++)
  {
    led->leds[ledsAvailable].fadeWait();
  }

}
