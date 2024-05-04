#pragma once

/*
* This RGBColor class defines a color using the RGB (Red, Green, Blue) components, 
* each represented by an unsigned char (r, g, b). 
* You can create instances of this class by passing the red, green, and blue values to the constructor.
* 
* RGBColor red(255, 0, 0); // Pure red color
* RGBColor green(0, 255, 0); // Pure green color
* RGBColor blue(0, 0, 255); // Pure blue color
* RGBColor custom(128, 64, 255); // Custom color with specified RGB values

*/

class RGBColor
{
public:
    unsigned char r, g, b;

    RGBColor(unsigned char red, unsigned char green, unsigned char blue)
        : r(red), g(green), b(blue)
    {
    }
};