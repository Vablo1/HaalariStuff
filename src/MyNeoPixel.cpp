#include "MyNeoPixel.h"

MyNeoPixel::MyNeoPixel(uint16_t n, uint8_t pin, neoPixelType t)
    : Adafruit_NeoPixel(n, pin, t) {}

void MyNeoPixel::setLetter(uint8_t index, uint32_t color)
{
    uint8_t str_len = strlen(text_order);

    if (index < str_len)
    {
        uint32_t firstnum = 0;

        for(int i = 0; i < index; i++)
        {
            firstnum += letters_map.at(text_order[i]);
        }

        fill(color, firstnum, letters_map.at(text_order[index]));
    }
}

void MyNeoPixel::setText(const char* text)
{
    text_order = text;
}
