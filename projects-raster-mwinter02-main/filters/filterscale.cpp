#include "canvas2d.h"
#include "filterutils.h"
#include "settings.h"

// g function
double g(double x, double a) {
    double radius;
    radius = (a<1) ? 1.0/a : 1.0;
    return (x>fabs(radius)) ? 0 : (1-fabs(x)/radius)/radius;
}

// hPrime for X direction
RGBA Canvas2D::hPrimeX(int k, double a, int row) {
    double redAcc = 0;
    double greenAcc = 0;
    double blueAcc = 0;
    double alphaAcc = 0;
    double weightsAcc = 0;
    double center = k/a + (1-a)/(2*a);
    double radius = (a>=1) ? 1 : 1/a;
    double left = std::ceil(center-radius);
    double right = std::floor(center+radius);

    for (int i = left; i <= right; ++i) {
        RGBA pixel = FilterUtils::getPixelRepeated(m_data, m_width, m_height, i, row);
        double weight = g(i-center, a);
        redAcc += weight * pixel.r;
        greenAcc += weight * pixel.g;
        blueAcc += weight * pixel.b;
        alphaAcc += weight * pixel.a;
        weightsAcc += weight;
    }
    RGBA result;
    result.r = static_cast<std::uint8_t>(redAcc/weightsAcc);
    result.g = static_cast<std::uint8_t>(greenAcc/weightsAcc);
    result.b = static_cast<std::uint8_t>(blueAcc/weightsAcc);
    result.a = static_cast<std::uint8_t>(alphaAcc/weightsAcc);
    return result;
}

// hPrime for Y direction
RGBA Canvas2D::hPrimeY(int k, double a, int col) {
    double redAcc = 0;
    double greenAcc = 0;
    double blueAcc = 0;
    double alphaAcc = 0;
    double weightsAcc = 0;
    double center = k/a + (1-a)/(2*a);
    double radius = (a>=1) ? 1 : 1/a;
    double left = std::ceil(center-radius);
    double right = std::floor(center+radius);
    for (int i = left; i <= right; ++i) {
        RGBA pixel = FilterUtils::getPixelRepeated(m_data, m_width, m_height, col, i);
        double weight = g(i - center, a);
        redAcc += weight * pixel.r;
        greenAcc += weight * pixel.g;
        blueAcc += weight * pixel.b;
        alphaAcc += weight * pixel.a;
        weightsAcc += weight;
    }
    RGBA result;
    result.r = static_cast<std::uint8_t>(redAcc/weightsAcc);
    result.g = static_cast<std::uint8_t>(greenAcc/weightsAcc);
    result.b = static_cast<std::uint8_t>(blueAcc/weightsAcc);
    result.a = static_cast<std::uint8_t>(alphaAcc/weightsAcc);
    return result;
}

// Applies scale filter
void Canvas2D::filterScale(float scaleX, float scaleY) {
    // Scale X
    std::vector<RGBA> dataX(m_height*int(m_width*scaleX));
    for (int y=0; y<m_height; y++) {
        for (int x=0; x<int(m_width*scaleX); x++) {
            dataX[y*int(m_width*scaleX)+x] = hPrimeX(x, scaleX, y);
        }
    }
    m_width = int(m_width * scaleX);
    m_data.resize(int(m_width)*int(m_height));
    for (int i=0; i<(int(m_width)*int(m_height)); i++) {
        m_data[i] = dataX[i];
    }

    // Scale Y
    std::vector<RGBA> dataY(int(m_height*scaleY) * m_width);
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < int(m_height * scaleY); y++) {
            dataY[y * m_width + x] = hPrimeY(y, scaleY, x);
        }
    }
    m_height = int(m_height * scaleY);
    m_data.resize(int(m_width) * int(m_height));
    for (int i=0; i<(int(m_width)*int(m_height)); i++) {
        m_data[i] = dataY[i];
    }
}

