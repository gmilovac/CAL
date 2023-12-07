#include <cstdint>
#include "src/canvas2d.h"
#include "filterutils.h"
#include "src/settings.h"

std::uint8_t rgbaToGray(const RGBA &pixel) {
    double r = pixel.r;
    double g = pixel.g;
    double b = pixel.b;
    std::uint8_t gray = (round( 0.299*r + 0.587*g + 0.114*b ));
    return gray;
}

void Canvas2D::filterGray() {
    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            size_t currentIndex = m_width * row + col;
            RGBA &currentPixel = m_data[currentIndex];
            std::uint8_t gray = rgbaToGray(currentPixel);
            currentPixel = RGBA{gray,gray,gray,255};
        }
    }
}
