#pragma once

#include <cstdint>

struct RGBA {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a = 255;
};

struct doubleRGBA {
    double r;
    double g;
    double b;
    double a=255;
};
