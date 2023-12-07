#include "filterutils.h"
#include "src/settings.h"
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

namespace FilterUtils {


// Repeats the pixel on the edge of the image such that A,B,C,D looks like ...A,A,A,B,C,D,D,D...
RGBA getPixelRepeated(std::vector<RGBA> &data, int width, int height, int x, int y) {
    int newX = (x < 0) ? 0 : std::min(x, width  - 1);
    int newY = (y < 0) ? 0 : std::min(y, height - 1);
    return data[width * newY + newX];
}

// Same as above but returns doubleRGBA
doubleRGBA getDoublePixelRepeated(std::vector<doubleRGBA> &data, int width, int height, int x, int y) {
    int newX = (x < 0) ? 0 : std::min(x, width  - 1);
    int newY = (y < 0) ? 0 : std::min(y, height - 1);
    return data[width * newY + newX];
}

/**
 * @brief convolve
 * @param data = vector<RGBA> representing image
 * @param width = width of image in pixels
 * @param height = height of image in pixels
 * @param xk = Horizontal kernel
 * @param yk = Vertical kernel
 * @return convolved image vector<RGBA>
 */
std::vector<RGBA> convolve(std::vector<RGBA> &data, int width, int height,
              const std::vector<double> &xk, const std::vector<double> &yk) {

    int imageSize = (width)*(height);
    std::vector<RGBA> result(imageSize); // initializes output vector same size as image
    std::vector<doubleRGBA> resultX(imageSize); // must be doubles to maintain accuracy

    for (int row=0;row < height; row++) { // horizontal convolution
        for (int col=0;col<width;col++) {

            double redAcc = 0.0;
            double greenAcc = 0.0;
            double blueAcc = 0.0;

            for (int i = (xk.size()-1);i>=0;i--) { // flips kernel

                int newX = col + (i - (xk.size()-1)/2);
                doubleRGBA pixel = rgbToDouble(getPixelRepeated(data,width,height,newX,row));
                redAcc += pixel.r*xk[i];
                greenAcc += pixel.g*xk[i];
                blueAcc += pixel.b*xk[i];
            }

            resultX.at(width*row+col) = doubleRGBA{redAcc,greenAcc,blueAcc,255.0};
        }                           //  Stores as doubleRGBA vector to maintain accuracy between convolutions
    }

    for (int row=0;row < height; row++) { // vertical convolution
        for (int col=0;col<width;col++) {

            double redAcc = 0.0;
            double greenAcc = 0.0;
            double blueAcc = 0.0;

            for (int i = (yk.size()-1);i>=0;i--) { // flips kernel

                int newY = row + (i - (yk.size()-1)/2);
                doubleRGBA pixel = getDoublePixelRepeated(resultX,width,height,col,newY);
                redAcc += pixel.r*yk[i];
                greenAcc += pixel.g*yk[i];
                blueAcc += pixel.b*yk[i];
            }

            if (settings.filterType == FilterType::FILTER_EDGE_DETECT) { // edge detect sensitivity
                redAcc*=settings.edgeDetectSensitivity;
                blueAcc*=settings.edgeDetectSensitivity;
                greenAcc*=settings.edgeDetectSensitivity;
            }

            result.at(width*row+col) = RGBA{doubleToUint8(redAcc),doubleToUint8(greenAcc),doubleToUint8(blueAcc),255};
        }
    }

    return result;
}


doubleRGBA rgbToDouble(RGBA input) {
    doubleRGBA result;
    result.r = static_cast<double>(input.r);
    result.g = static_cast<double>(input.g);
    result.b = static_cast<double>(input.b);
    result.a = static_cast<double>(input.a);
    return result;
}

RGBA doubleToRGB(doubleRGBA input) {
    RGBA result;
    result.r = doubleToUint8(input.r);
    result.g = doubleToUint8(input.g);
    result.b = doubleToUint8(input.b);
    result.a = doubleToUint8(input.a);
    return result;
}

std::uint8_t doubleToUint8(double input) {
    input = (input<0) ? 0 : input; // negative values set to 0
    return (input>255) ? 255 : input; // positive values >255 set to 255
}

}

