#ifndef MYNEOPIXEL_H
#define MYNEOPIXEL_H

#include <Adafruit_NeoPixel.h>
#include <map>
#include <cstring>


class MyNeoPixel : public Adafruit_NeoPixel {
private:

    //map for led count in each letter
    const std::map<char, int> letters_map = {
        {'A', 21},
        {'E', 25},
        {'L', 15},
        {'O', 20},
        {'T', 15},
        {'V', 20}
    };

    // Variable for the actual order of letters
    const char* text_order;

public:
    MyNeoPixel(uint16_t n, uint8_t pin, neoPixelType t);
    void setText(const char* text);
    void setLetter(uint8_t index, uint32_t color);
};

#endif