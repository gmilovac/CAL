#include "src/canvas2d.h"
#include "filterutils.h"
#include "src/settings.h"

std::vector<RGBA> median1D(std::vector<RGBA> &data, int width, int height) {
    std::vector<doubleRGBA> resultX(width * height);
    std::vector<RGBA> result(width * height);
    int r = settings.medianRadius;
    int size = 2*r+1;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {

            std::vector<int> redMedian(size);
            std::vector<int> greenMedian(size);
            std::vector<int> blueMedian(size);

            for (int i = 0; i < 2*r+1; i++) {
                int newY = col+i-r;
                RGBA pixel = FilterUtils::getPixelRepeated(data, width, height, newY, row);
                redMedian.at(i) = pixel.r;
                greenMedian.at(i) = pixel.g;
                blueMedian.at(i) = pixel.b;
            }

            std::sort(redMedian.begin(), redMedian.end());
            std::sort(greenMedian.begin(), greenMedian.end());
            std::sort(blueMedian.begin(), blueMedian.end());

            resultX[row*width + col].r = redMedian.at(r);
            resultX[row*width + col].g = greenMedian.at(r);
            resultX[row*width + col].b = blueMedian.at(r);
            resultX[row*width + col].a = 255.f;
        }
    }
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {

            std::vector<int> redMedian(size);
            std::vector<int> greenMedian(size);
            std::vector<int> blueMedian(size);

            for (int i = 0; i < size; i++) {
                int newX = row+i-r;
                doubleRGBA pixel = FilterUtils::getDoublePixelRepeated(resultX, width, height, col, newX);
                redMedian.at(i) = pixel.r;
                greenMedian.at(i) = pixel.g;
                blueMedian.at(i) = pixel.b;
            }

            std::sort(redMedian.begin(), redMedian.end());
            std::sort(greenMedian.begin(), greenMedian.end());
            std::sort(blueMedian.begin(), blueMedian.end());

            result[row*width + col].r = static_cast<uint8_t>(redMedian.at(r));
            result[row*width + col].g = static_cast<uint8_t>(greenMedian.at(r));
            result[row*width + col].b = static_cast<uint8_t>(blueMedian.at(r));
            result[row*width + col].a = static_cast<uint8_t>(255.f);
        }
    }
    return result;
}

void Canvas2D::filterMedian() {
        std::vector<RGBA> result = median1D(m_data, m_width, m_height);
        for (int k = 0; k < result.size(); k++) {
            m_data[k] = result[k];
        }
}
