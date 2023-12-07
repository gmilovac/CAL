//
// Created by Gordan Milovac on 12/6/23.
//

#include "canvas2d.h"
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include "settings.h"

/**
 * @brief Initializes new 500x500 canvas
 */
void Canvas2D::init() {
    setMouseTracking(true);
    m_width = 500;
    m_height = 500;
    clearCanvas();
}

/**
 * @brief Canvas2D::clearCanvas sets all canvas pixels to blank white
 */
void Canvas2D::clearCanvas() {
    m_data.assign(m_width * m_height, RGBA{255, 255, 255, 255});
    settings.imagePath = "";
    displayImage();
}

/**
 * @brief Stores the image specified from the input file in this class's
 * `std::vector<RGBA> m_image`.
 * Also saves the image width and height to canvas width and height respectively.
 * @param file: file path to an image
 * @return True if successfully loads image, False otherwise.
 */
bool Canvas2D::loadImageFromFile(const QString &file) {
    QImage myImage;
    if (!myImage.load(file)) {
        std::cout<<"Failed to load in image"<<std::endl;
        return false;
    }
    myImage = myImage.convertToFormat(QImage::Format_RGBX8888);
    m_width = myImage.width();
    m_height = myImage.height();
    QByteArray arr = QByteArray::fromRawData((const char*) myImage.bits(), myImage.sizeInBytes());

    m_data.clear();
    m_data.reserve(m_width * m_height);
    for (int i = 0; i < arr.size() / 4.f; i++){
        m_data.push_back(RGBA{(std::uint8_t) arr[4*i], (std::uint8_t) arr[4*i+1], (std::uint8_t) arr[4*i+2], (std::uint8_t) arr[4*i+3]});
    }
    displayImage();
    return true;
}

/**
 * @brief Saves the current canvas image to the specified file path.
 * @param file: file path to save image to
 * @return True if successfully saves image, False otherwise.
 */
bool Canvas2D::saveImageToFile(const QString &file) {
    QImage myImage = QImage(m_width, m_height, QImage::Format_RGBX8888);
    for (int i = 0; i < m_data.size(); i++){
        myImage.setPixelColor(i % m_width, i / m_width, QColor(m_data[i].r, m_data[i].g, m_data[i].b, m_data[i].a));
    }
    if (!myImage.save(file)) {
        std::cout<<"Failed to save image"<<std::endl;
        return false;
    }
    return true;
}


/**
 * @brief Get Canvas2D's image data and display this to the GUI
 */
void Canvas2D::displayImage() {
    QByteArray* img = new QByteArray(reinterpret_cast<const char*>(m_data.data()), 4*m_data.size());
    QImage now = QImage((const uchar*)img->data(), m_width, m_height, QImage::Format_RGBX8888);
    setPixmap(QPixmap::fromImage(now));
    setFixedSize(m_width, m_height);
    update();
}

/**
 * @brief Canvas2D::resize resizes canvas to new width and height
 * @param w
 * @param h
 */
void Canvas2D::resize(int w, int h) {
    m_width = w;
    m_height = h;
    m_data.resize(w * h);
    displayImage();
}

/**
 * @brief Called when the filter button is pressed in the UI
 */
void Canvas2D::filterImage() {
    switch(settings.filterType) {
        case FilterType::FILTER_SCALE: {
            filterScale(settings.scaleX,settings.scaleY);
            break;
        }
        case FilterType::FILTER_EDGE_DETECT: {
            filterEdge();
            break;
        }

        case FilterType::FILTER_MEDIAN: {
            filterMedian();
            break;
        }

        case FilterType::FILTER_CHROMATIC: {
            filterChroma();
            break;
        }
        case FilterType::FILTER_BLUR: default:{
            filterBlur(false); // true = gaussian blur, false = triangle blur
            break;
        }
    }
    displayImage();
}

/**
 * @brief Called when any of the parameters in the UI are modified.
 */
void Canvas2D::settingsChanged() {
    // this saves your UI settings locally to load next time you run the program
    settings.saveSettings();

}


//mouse down command, sets boolean m_isDown to true
void Canvas2D::mouseDown(int x, int y) {
    // Brush TODO
    if (BrushType(settings.brushType) == BRUSH_SMUDGE) {
        captureSmudge(getMask(x,y));
    }
    m_isDown=true;
    editCanvas(x,y);
    displayImage();
}

//mouse dragged command, updates canvas and edits
void Canvas2D::mouseDragged(int x, int y) {
    // Brush TODO
    if (m_isDown) {
        editCanvas(x,y);
        displayImage();
        if (BrushType(settings.brushType) == BRUSH_SMUDGE) {
            captureSmudge(getMask(x,y));
        }
    }
}

//Mouse up command
void Canvas2D::mouseUp(int x, int y) {
    m_isDown=false;
    visitedPixels.clear();
}


//Edits the canvas
void Canvas2D::editCanvas(int x, int y) {
    std::vector<int> mask = getMask(x,y);
    for (int j=0;j<mask.size();j++) {
        int i = mask.at(j);
        if (i>=0 && i<(m_width*m_height)) {
            if (settings.brushType==BRUSH_SMUDGE) {
                blendColor(smudgeMask.at(j),x,y,i);

            }
            else {
                if (settings.fixAlphaBlending) {
                    if (!visitedPixels.contains(i)) {
                        blendColor(settings.brushColor,x,y,i);
                        visitedPixels.insert(i);
                    }
                }
                else {
                    RGBA brushColor = settings.brushColor;
                    blendColor(brushColor,x,y,i);
                }
            }
        }
    }
}

//Calculates the final color from a given brushColor and the canvas location
void Canvas2D::blendColor(RGBA brushColor,int x,int y, int i) {
    RGBA canvasColor = m_data.at(i);
    int xi = indexToX(i);
    int yi = indexToY(i);
    float M = getMaskAlpha(x,y,xi,yi);
    float a = static_cast<float>(brushColor.a) / 255;
    if (xi>=0 && xi<m_width && yi>=0 && yi<m_height) {
        m_data.at(i).r = round((M*a)*brushColor.r+(1-(M*a))*canvasColor.r);
        m_data.at(i).g = round((M*a)*brushColor.g+(1-(M*a))*canvasColor.g);
        m_data.at(i).b = round((M*a)*brushColor.b+(1-(M*a))*canvasColor.b);
    }
}

// Captures a smudge based on pixels currently in view of mask
void Canvas2D::captureSmudge(std::vector<int> indexes) {
    std::vector<RGBA> smudged;
    for (int i : indexes) {

        smudged.push_back(m_data.at(i));
    }
    smudgeMask = smudged;
}


int Canvas2D::indexToX(int index) {
    return index % m_width;
}

int Canvas2D::indexToY(int index) {
    return floor(index/m_width);
}

//Gets the alpha value for a given position depending on the mask in effect
float Canvas2D::getMaskAlpha(int x, int y, int x2, int y2) {
    BrushType b = BrushType(settings.brushType);
    int r = settings.brushRadius;
    float M = 1.0;
    switch(b) {

        case BRUSH_QUADRATIC: {
            //f(d) = d^2 - 2d + 1 gives us f(0) = 1, f(0.5) = 0.25, f(1) = 0
            float dr = sqrt((pow(x-x2,2)+pow(y-y2,2)))/r; //dr is the ratio of the distance to the radius
            if (dr>=0 && dr<=1) {
                M = pow(dr,2) - 2*dr + 1;
            }
            else {
                M = 0.0;
            }
            break;
        }

        case BRUSH_LINEAR: case BRUSH_SMUDGE: {
            float d = sqrt(pow(x-x2,2)+pow(y-y2,2));
            M = 1 - (std::max(d,static_cast<float> (0)))/r;
            break;
        }

        default: {
            M = 1.0;
        }

    }

    return M;
}

// Gets a vector of indexes for a given mask, centered at x,y of radius settings.brushRadius
std::vector<int> Canvas2D::getMask(int x, int y) {
    if (x>=0 && x<m_width && y>=0 && y<m_height) { // create a square mask from (x-r,y-r) to (x+r,y+r)
        std::vector<int> mask;
        int r = settings.brushRadius;
        for (int i = x-r; i<x+r;i++) {
            for (int j=y-r; j<y+r; j++) { // check if pixel in mask falls within circle of brush radius
                if (i>=0 && i<m_width && j>=0 && j<m_height) { // stops brush from looping to other side when painting on edge
                    if (pow(x-i,2) + pow(y-j,2) <= pow(r,2)) {
                        mask.push_back(posToIndex(i,j)); // adds pixel to brush mask vector if it falls within brush circle

                    }
                }
            }

        }
        return mask;
    }

    else { // returns empty vector is center of brush is outside of canvas
        std::vector<int> empty;
        return empty;
    }
}


//position to index helper method
int Canvas2D::posToIndex(int x, int y) {
    int index = (m_width*y)+x;
    return index;
}
