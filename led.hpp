#include <stdint.h> //libary which includes uint8_t etc.
//compiler params:
// g++ -std=c++0x -o object led.hpp led.cpp -lpigpio -lrt -lpthread && ./object


class LED
{
  public:
    //constructor
    LED(uint16_t led_pin, bool led_isColor, uint16_t led_currentBrightness, uint16_t led_argetBrightness);
    //getter
    uint16_t getPin();
    bool getIsColor();
    uint16_t getCurrentBrightenss();
    uint16_t getTargetBrightness();

    //setter
    void setPin(uint16_t newpin);
    void setIsColor(bool newisColor);
    void setCurrentBrightenss(uint16_t new_cBrightness);
    void setTargetBrightness(uint16_t new_tBrightness);
    //functions
    void writeBrightnessToPin(uint16_t brightness);

  private:
    uint16_t pin;
    bool isColor;
    uint16_t currentBrightness;
    uint16_t targetBrightness;
};
