#include "canvas2d.h"
#include "filterutils.h"
#include "settings.h"
#include <iostream>

// Returns sqrt(A^2 + B^2) rounded to an integer
int pythag(double A, double B) {
    return int(round(sqrt(pow(A,2)+pow(B,2))));
}
// applies edge detection filter
void Canvas2D::filterEdge() {
    filterGray();

    std::vector<double> vGx{1.0, 2.0, 1.0};
    std::vector<double> hGx{-1.0, 0.0, 1.0};

    std::vector<double> vGy{-1.0, 0.0, 1.0};
    std::vector<double> hGy{1.0, 2.0, 1.0};

    std::vector<RGBA> Gx = FilterUtils::convolve(m_data,m_width,m_height,vGx,hGx);
    std::vector<RGBA> Gy = FilterUtils::convolve(m_data,m_width,m_height,vGy,hGy);

        for (int i = 0; i<Gx.size();i++) {
            int v = pythag(Gx[i].r,Gy[i].r);
            std::uint8_t g = std::min(255,v); // clamping to 255
            m_data[i] = RGBA{g,g,g,255};
        }
}
