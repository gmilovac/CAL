//
// Created by Gordan Milovac on 12/6/23.
//

#ifndef CAL_FILTERUTILS_H
#define CAL_FILTERUTILS_H

#pragma once

#include "src/rgba.h"
#include <cstdint>
#include <vector>

namespace FilterUtils {

    inline std::uint8_t floatToUint8(float x);
    inline float uint8ToFloat(std::uint8_t x);

    void convolve2D(std::vector<RGBA>& data,
                    int width,
                    int height,
                    const std::vector<float>& kernel);

    std::vector<RGBA> convolve(std::vector<RGBA>& data,int width, int height,
                               const std::vector<double>& xKernel, const std::vector<double>& yKernel);


    std::vector<RGBA> convolve1D(std::vector<RGBA> &data, int width, int height, const std::vector<double> &kernelX,
                                 const std::vector<double> &kernelY);

    RGBA getPixelRepeated(std::vector<RGBA> &data, int width, int height, int x, int y);
    doubleRGBA getDoublePixelRepeated(std::vector<doubleRGBA> &data, int width, int height, int x, int y);
    doubleRGBA rgbToDouble(RGBA input);
    RGBA doubleToRGB(doubleRGBA input);
    std::uint8_t doubleToUint8(double input);
    double uint8ToDouble(std::uint8_t input);
}


#endif //CAL_FILTERUTILS_H
