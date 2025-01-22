#ifndef LIGHTSETTINGS_H
#define LIGHTSETTINGS_H

#include <cstdint>
#include <vector>


class ColorState {
private:
    std::vector<uint16_t> colors_list;

public: 
    uint8_t brightness;
    uint8_t mode;

    ColorState();
    ~ColorState();

    void addColor(uint16_t color);
    uint16_t getColor(uint16_t index);
    void clearColor();
    uint16_t colorCount();
    uint8_t setState(uint8_t* data, uint16_t size);
    std::vector<uint8_t> getData();
};

#endif
