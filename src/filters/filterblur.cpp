#include "src/canvas2d.h"
#include "filterutils.h"
#include "src/settings.h"
#include <iostream>



std::vector<double> createTriangleKernel(int r) {
    int size = 2*r+1;
    std::vector<double> k(size);
    for (int i=0; i<=r;i++) {
        k[i] = static_cast<double>(i+1);
        k[size-i-1] = k[i]; // makes distribution symetrical
    }
    //normalize kernel
    double sum = 0.0;
    for (int i=0;i<size;i++) {
        sum += k[i];
    }
    for (int i=0;i<size;i++) {
        k[i] = k[i]/sum;
    }
    return k;
}

// f(x) = 1/(sqrt(2*pi*σ^2)) * e^(-(x^2)/(2*σ^2))
std::vector<double> createGausianKernel(int r) {
    int size = 2*r+1;
    std::vector<double> k(size);
    double sd = (static_cast<double>(r))/3;
    for (int i=0;i<=r;i++) {
        double x = r - i;
        k[i] = (1/(sqrt(2*M_PI*pow(sd,2))))*(exp(-(pow(x,2))/(2*pow(sd,2))));
        k[size-i-1]=k[i]; // makes distribution symetrical
    }
    //normalize kernel
    float sum = 0.0f;
    for (int i=0;i<size;i++) {
        sum += k[i];
    }
    for (int i=0;i<size;i++) {
        k[i] = k[i]/sum;
    }
    return k;
}

// FilterBlur method alters m_data to be blurred
void Canvas2D::filterBlur(bool isGaussian) {
    std::vector<double> tblur;
    tblur = (isGaussian) ? createGausianKernel(settings.blurRadius) : createTriangleKernel(settings.blurRadius);
    m_data = FilterUtils::convolve(m_data,m_width,m_height,tblur,tblur);
}



//2D triangle blur kernel (not used)
//std::vector<float> create2DTriangleBlurKernel(int r) {
//    // Calculate the size of the kernel
//    int kernelSize = 2 * r + 1;

//    // Initialize the kernel with zeros
//    std::vector<std::vector<float>> kernel(kernelSize, std::vector<float>(kernelSize, 0.0f));

//    // Calculate the center of the kernel
//    int centerX = r;
//    int centerY = r;

//    // Fill the kernel with values for the triangle blur
//    for (int i = 0; i < kernelSize; ++i) {
//        for (int j = 0; j < kernelSize; ++j) {
//            // Calculate the distance from the center
//            int distanceX = std::abs(centerX - i);
//            int distanceY = std::abs(centerY - j);

//            // Calculate the value for the triangle blur
//            float value = std::max(0, r - distanceX) * std::max(0, r - distanceY);

//            // Store the value in the kernel
//            kernel[i][j] = value;
//        }
//    }

//    // Normalize the kernel
//    float sum = 0.0f;
//    for (int i = 0; i < kernelSize; ++i) {
//        for (int j = 0; j < kernelSize; ++j) {
//            sum += kernel[i][j];
//        }
//    }

//    for (int i = 0; i < kernelSize; ++i) {
//        for (int j = 0; j < kernelSize; ++j) {
//            kernel[i][j] /= sum;
//        }
//    }

//    //flatten 2d vector to 1d
//    size_t numRows = kernel.size();
//    size_t numCols = (numRows > 0) ? kernel[0].size() : 0;
//    size_t totalElements = numRows * numCols;

//    std::vector<float> flatKernel(totalElements);

//    for (size_t i = 0; i < numRows; ++i) {
//        for (size_t j = 0; j < numCols; ++j) {
//            flatKernel[i * numCols + j] = kernel[i][j];
//        }
//    }

//    return kernel[r];
//}
