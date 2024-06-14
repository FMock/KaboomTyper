#ifndef RGBCOLOR_H
#define RGBCOLOR_H

#include <array>
#include <string>

class RGBColor
{
public:
    unsigned char r, g, b;

    RGBColor(unsigned char red, unsigned char green, unsigned char blue)
        : r(red), g(green), b(blue)
    {
    }

    enum ColorName
    {
        White,
        Black,
        Red,
        Green,
        Blue,
        Yellow,
        Cyan,
        Magenta,
        Gray,
        DarkRed,
        DarkGreen,
        DarkBlue,
        Orange,
        Pink,
        Purple,
        Brown,
        Lime,
        Olive,
        Navy,
        Teal,
        Maroon,
        Silver,
        Gold,
        LightBlue,
        LightGreen,
        LightGray,
        DarkGray,
        ColorCount // This should always be the last element to count the number of colors
    };

    static const RGBColor& GetRGBColor(ColorName colorName);

private:
    static const std::array<RGBColor, ColorCount> colors;
};

#endif // RGBCOLOR_H
