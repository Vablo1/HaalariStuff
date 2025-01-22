#include "ColorState.h"

ColorState::ColorState() {}

ColorState::~ColorState() {}

void ColorState::addColor(uint16_t color)
{
    colors_list.push_back(color);
}

uint16_t ColorState::getColor(uint16_t index)
{
    if (index < colors_list.size())
    {
        return colors_list[index];
    }
    return 0;
}

void ColorState::clearColor()
{
    colors_list.clear();
}

uint16_t ColorState::colorCount()
{
    return colors_list.size();
}

uint8_t ColorState::setState(uint8_t* data, uint16_t size)
{
    if (size < 1) return 0;

    if (data[0] == 0)
    {
        mode = 0;
        brightness = 0;
        clearColor();
        return 1;
    }

    if (size > 3)
    {
        if (data[0] == 2)
        {
            if (mode != 1) return 0;
        }

        if (data[0] == 1 || data[0] == 2)
        {
            if (data[0] == 1)
            {
                mode = 1;
                clearColor();
                brightness = data[1];
            }
            
            for(int i = 2; i < size; i+=2)
            {
                addColor(data[i] << 8 | data[i+1]);
            }

            return 1;
        }

        if (data[0] == 3)
        {
            mode = 3;
            brightness = data[1];

            clearColor();
            addColor(data[2] << 8 | data[3]);

            return 1;
        }
    }

    return 0;
}

std::vector<uint8_t> ColorState::getData()
{
    std::vector<uint8_t> out;
    out.push_back(mode);
    out.push_back(brightness);
    
    uint16_t size = colors_list.size();
    for (int i = 0; i < size; i++)
    {
        out.push_back(colors_list[i & 0xFF]);
        out.push_back((colors_list[i] & 0xFF00) >> 8);
    }

    return out;
}