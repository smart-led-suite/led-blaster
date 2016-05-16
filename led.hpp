#ifndef _led_h
#define _led_h
#include <stdint.h> //libary which includes uint8_t etc.
#include <string>
//compiler params:
// g++ -std=c++0x -o object led.hpp led.cpp init.cpp -lpigpio -lrt -lpthread

class LED
{
  public:
    //constructor
    LED(std::string led_colorcode, uint16_t led_pin, bool led_isColor, int led_currentBrightness, int led_argetBrightness);
    //destructor
    ~LED(void);
    //getter
    std::string getColorCode();
    uint16_t getPin();
    bool IsColor();
    bool isFading();
    int getCurrentBrightness();
    int getTargetBrightness();
    static int getPwmSteps();
    static int getFadeTime();
    pthread_t getFadeThread();
    //setter
    static void setFadeTime(int newFadeTime);
    void setColorCode(std::string new_colorcode);
    void setPin(uint16_t newpin);
    void setIsColor(bool newisColor);
    //this also writes the brightness to the pin
    void setCurrentBrightness(int new_cBrightness);
    //this only "prepares" it because to apply it you have to call fadeInThread()
    void setTargetBrightness(int new_tBrightness);
    //functions
    //call this function to fade in a thread
    void fadeInThread(void); //fades via threads
    //internal fade function. NOT in a thread
    void * fade(void);
    //cancel fade
    void fadeCancel(void);
    //wait until fade is done
    void fadeWait(void);

  private:
    struct fadeThreadStruct {
      LED * led;
      int fadeTime;
    };
    //internal thread launcher
    static void * fadeLauncher(void *context);
    int initPin(void);
    pthread_t fadeThread;
    static int fadeTime;
    bool fading;
    std::string colorcode;
    uint16_t pin;
    bool isColor;
    int currentBrightness;
    int targetBrightness;
    static int pwmSteps;
};
#endif //EOF
