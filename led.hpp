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
    int getPwmSteps();
    pthread_t getFadeThread();
    //setter
    void setColorCode(std::string new_colorcode);
    void setPin(uint16_t newpin);
    void setIsColor(bool newisColor);
    void setCurrentBrightness(int new_cBrightness);
    void setTargetBrightness(int new_tBrightness);
    //functions
    void fadeInThread(int fadeTime); //fades via threads
    void * fade(void * fadeTime);

  private:
    struct fadeThreadStruct {
    //  LED * led;
      int fadeTime;
    };
    static void * fadeLauncher(void *context);
    int initPin(void);
    pthread_t fadeThread;
    bool fading;
    std::string colorcode;
    uint16_t pin;
    bool isColor;
    int currentBrightness;
    int targetBrightness;
    int pwmSteps;
};
#endif //EOF
