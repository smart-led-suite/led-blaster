#ifndef _led_h
#define _led_h
#include <stdint.h> //libary which includes uint8_t etc.
#include <string>
#include <map>
#include <memory> //shared pointer

namespace led {
  class LED
  {
    public:
      //constructor
      LED(std::string led_colorcode, uint16_t led_pin, bool led_isColor, int led_currentBrightness, int led_argetBrightness, int led_trueColorMultiplier);
      //destructor
      ~LED(void);
      //static led map for every led
      static std::map<int, led::LED*> ledMap;
      //*********getter************************
      std::string getColorCode();
      uint16_t getPin();
      bool IsColor();
      bool isFading();
      bool isRandomlyFading();
      int getCurrentBrightness();
      int getTargetBrightness();
      int getTrueColorMultipier();
      static int getPwmSteps();
      static int getFadeTime();
      pthread_t getFadeThread();
      //************setter*********************+
      static void setFadeTime(int newFadeTime);
      void setColorCode(std::string new_colorcode);
      void setPin(uint16_t newpin);
      void setIsColor(bool newisColor);
      void setTrueColorMultipier(int new_Multiplier);
      //this also writes the brightness to the pin
      void setCurrentBrightness(int new_cBrightness);
      //this only "prepares" it because to apply it you have to call fadeInThread()
      void setTargetBrightness(int new_tBrightness);
      //***********functions****************
      //turns all leds instantly off
      static void turnAllLedsOff(void);
      //fades all leds with fadeTime off
      static void fadeAllLedsOff(void);
      static bool initGeneral(void); //initializes the libary
      //call this function to fade in a thread
      void fadeInThread(void); //also cancels running threads
      //internal fade function. NOT in a thread
      void * fade(void);
      //cancel fade
      void fadeCancel(void);
      //wait until fade is done
      void fadeWait(void);
      //fade randomly
      void fadeRandomInThread(void);
    private:
      //internal thread launcher
      static void * fadeLauncher(void *context);
      static void * fadeRandom(void *context);
      //initializes pin. called by constructor
      int initPin(void);
      //number in % that lowers the brightness of a specific color
      int trueColorMultiplier;
      pthread_t fadeThread;
      static int fadeTime;
      volatile bool fading;
      volatile bool randomlyFading;
      std::string colorcode;
      uint16_t pin;
      bool isColor;
      int currentBrightness;
      int targetBrightness;
      static int pwmSteps;
      static int naturalSteps[1000];
  };
}
#endif //EOF
