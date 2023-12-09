//
// Created by Gordan Milovac on 12/6/23.
//

#include "canvas2d.h"
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <stack>
#include "settings.h"
#include "mainwindow.h"

/**
 * @brief Initializes new 500x500 canvas
 */
void Canvas2D::init() {
    setMouseTracking(true);
    m_width = 512;
    m_height = 512;
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
    myImage = myImage.copy(0, 0, m_width, m_height); // Crops image to m_width x m_height from top left
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



///**
// * @brief Canvas2D::resize resizes canvas to new width and height
// * @param w
// * @param h
// */
//void Canvas2D::resize(int w, int h) {
//    m_width = w;
//    m_height = h;
//    m_data.resize(w * h);
//    displayImage();
//}

/**
 * @brief Called when any of the parameters in the UI are modified.
 */
void Canvas2D::settingsChanged() {
    // this saves your UI settings locally to load next time you run the program
    settings.saveSettings();
}

// Mouse down command, sets boolean m_isDown to true
void Canvas2D::mouseDown(int x, int y) {
    if (BrushType(settings.brushType) == BRUSH_FILL) {
        fill(m_data[posToIndex(x,y)], x, y, 10);
    }
    if (BrushType(settings.brushType) != BRUSH_FILL) {
    m_isDown=true;
    editCanvas(x,y);
    }
    displayImage();
}

void Canvas2D::fill(RGBA col, int x, int y, int depth) {
    std::stack<std::pair<int, int>> pixelsToVisit;
    pixelsToVisit.push(std::make_pair(x, y));
    if (rgbEquals(settings.brushColor,col)){
    return;
    }
    while (!pixelsToVisit.empty()) {
    auto [xx, yy] = pixelsToVisit.top();
    pixelsToVisit.pop();
    if (xx < 0 || xx >= m_width || yy < 0 || yy >= m_height || !rgbEquals(m_data[posToIndex(xx,yy)],col)) {
        continue;
    }
    m_data[yy * m_width + xx] = settings.brushColor;
    pixelsToVisit.push(std::make_pair(xx + 1, yy));
    pixelsToVisit.push(std::make_pair(xx - 1, yy));
    pixelsToVisit.push(std::make_pair(xx, yy + 1));
    pixelsToVisit.push(std::make_pair(xx, yy - 1));
    }
}

std::vector<float> Canvas2D::blurImage(std::vector<float> input) {
    std::vector<float> tblur = createGausianKernel(settings.blurRadius);
    std::vector<float> output(input.size());
    output = convolve(input,m_width,m_height,tblur,tblur);
//    for (int i=0; i<input.size(); i++) {
//    m_data[i].r = fToInt(255.f*output[i]);
//    m_data[i].g = fToInt(255.f*output[i]);
//    m_data[i].b = fToInt(255.f*output[i]);
//    m_data[i].a = 255;
//    }

   // displayImage();
    return output;
}

std::vector<float> Canvas2D::createGausianKernel(int r) {
    int size = 2*r+1;
    std::vector<float> k(size);
    float sd = (static_cast<float>(r))/3;
    for (int i=0;i<=r;i++) {
    float x = r - i;
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

std::vector<float> Canvas2D::getHeightMap() {
    std::vector<float> heightMap(m_data.size());
    for (int i=0; i<heightMap.size(); i++) {
    RGBA color = m_data[i];
    if (rgbEquals(color, MainWindow::MOUNTAINS_COLOR)) {
        heightMap[i] = 0.2f;
    }
    else if (rgbEquals(color, MainWindow::FOREST_COLOR)) {
        heightMap[i] = 0.08f;
    }
    else if (rgbEquals(color, MainWindow::GRASSLAND_COLOR)) {
        heightMap[i] = 0.1f;
    }
    else if (rgbEquals(color, MainWindow::DESERT_COLOR)) {
        heightMap[i] = 0.05f;
    }
    else if (rgbEquals(color, MainWindow::LAKE_COLOR)) {
        heightMap[i] = 0.0f;
    }
    else if (rgbEquals(color, MainWindow::OCEAN_COLOR)) {
        heightMap[i] = 0.0f;
    }
    else {
        heightMap[i] = 0.0f;
    }

    //m_data[i] = RGBA{fToInt(heightMap[i]*255.f),fToInt(heightMap[i]*255.f),fToInt(heightMap[i]*255.f),255};
    }

    return heightMap;
    //displayImage();

}
std::vector<float> Canvas2D::getNoiseMap() {
    std::vector<float> noiseMap(m_data.size());
    for (int i=0; i<noiseMap.size(); i++) {
    RGBA color = m_data[i];
    if (rgbEquals(color, MainWindow::MOUNTAINS_COLOR)) {
        noiseMap[i] = 1.f;
    }
    else if (rgbEquals(color, MainWindow::FOREST_COLOR)) {
        noiseMap[i] = 0.1f;
    }
    else if (rgbEquals(color, MainWindow::GRASSLAND_COLOR)) {
        noiseMap[i] = 0.2f;
    }
    else if (rgbEquals(color, MainWindow::DESERT_COLOR)) {
        noiseMap[i] = 0.3f;
    }
    else if (rgbEquals(color, MainWindow::LAKE_COLOR)) {
        noiseMap[i] = 0.0f;
    }
    else if (rgbEquals(color, MainWindow::OCEAN_COLOR)) {
        noiseMap[i] = 0.f;
    }
    else {
        noiseMap[i] = 0.0f;
    }

    //m_data[i] = RGBA{fToInt(noiseMap[i]*255.f),fToInt(noiseMap[i]*255.f),fToInt(noiseMap[i]*255.f),255};
    }
    //displayImage();
    return noiseMap;
}

std::vector<float> Canvas2D::convolve(std::vector<float> &data, int width, int height,
                           const std::vector<float> &xk, const std::vector<float> &yk) {

    int imageSize = (width)*(height);
    std::vector<float> result(imageSize); // initializes output vector same size as image
    std::vector<float> resultX(imageSize); // must be doubles to maintain accuracy

    for (int row=0;row < height; row++) { // horizontal convolution
    for (int col=0;col<width;col++) {

        float acc = 0.0;
//        float greenAcc = 0.0;
//        float blueAcc = 0.0;

        for (int i = (xk.size()-1);i>=0;i--) { // flips kernel

            int newX = col + (i - (xk.size()-1)/2);
            float pixel = (getPixelRepeated(data,width,height,newX,row));
            acc += pixel*xk[i];
//            greenAcc += pixel.g*xk[i];
//            blueAcc += pixel.b*xk[i];
        }

        resultX.at(width*row+col) = acc;
    }                           //  Stores as doubleRGBA vector to maintain accuracy between convolutions
    }

    for (int row=0;row < height; row++) { // vertical convolution
    for (int col=0;col<width;col++) {

        float acc = 0.0;
//        float greenAcc = 0.0;
//        float blueAcc = 0.0;

        for (int i = (yk.size()-1);i>=0;i--) { // flips kernel

            int newY = row + (i - (yk.size()-1)/2);
            float pixel = getPixelRepeated(resultX,width,height,col,newY);
            acc += pixel*yk[i];
//            greenAcc += pixel.g*yk[i];
//            blueAcc += pixel.b*yk[i];
        }

        result.at(width*row+col) = acc;
    }
    }

    return result;
}

int Canvas2D::fToInt(float x) {
    return std::max(std::min(int(round(x)), 255), 0);
}

float Canvas2D::getPixelRepeated(std::vector<float> &data, int width, int height, int x, int y) {
    int newX = (x < 0) ? 0 : std::min(x, width  - 1);
    int newY = (y < 0) ? 0 : std::min(y, height - 1);
    return data[width * newY + newX];
}

bool Canvas2D::rgbEquals(RGBA a, RGBA b) {
    if (a.r == b.r) {
        if (a.g == b.g) {
            if (a.b == b.b) {
                return true;
            }
        }
    }
    return false;
}

// Mouse dragged command, updates canvas and edits
void Canvas2D::mouseDragged(int x, int y) {
    if (m_isDown) {
        editCanvas(x,y);
        displayImage();
    }
}

// Mouse up command
void Canvas2D::mouseUp(int x, int y) {
    m_isDown=false;
    visitedPixels.clear();
}

// Edits the canvas
void Canvas2D::editCanvas(int x, int y) {
    std::vector<int> mask = getMask(x,y);
    for (int j=0;j<mask.size();j++) {
        int i = mask.at(j);
        if (i>=0 && i<(m_width*m_height)) {
            RGBA brushColor = settings.brushColor;
            blendColor(brushColor,x,y,i);
        }
    }
}

// Calculates the final color from a given brushColor and the canvas location
void Canvas2D::blendColor(RGBA brushColor,int x,int y, int i) {
    RGBA canvasColor = m_data.at(i);
    int xi = i % m_width;
    int yi = floor(i / m_width);
    float a = static_cast<float>(brushColor.a) / 255;
    if (xi>=0 && xi<m_width && yi>=0 && yi<m_height) {
        m_data.at(i).r = round(a*brushColor.r+(1-a)*canvasColor.r);
        m_data.at(i).g = round(a*brushColor.g+(1-a)*canvasColor.g);
        m_data.at(i).b = round(a*brushColor.b+(1-a)*canvasColor.b);
    }
}

std::vector<glm::vec4> Canvas2D::getCanvasData() {
    std::vector<glm::vec4> d(m_data.size());
    for (int i=0; i<m_data.size(); i++) {
        d[i][0] = (float(m_data[i].r)/255.f);
        d[i][1] = (float(m_data[i].g)/255.f);
        d[i][2] = (float(m_data[i].b)/255.f);
        d[i][3] = 1.f;
    }
    return d;
}

// Gets a vector of indexes for a given mask, centered at x,y of radius settings.brushRadius
std::vector<int> Canvas2D::getMask(int x, int y) {
    if (x>=0 && x<m_width && y>=0 && y<m_height) { // create a square mask from (x-r,y-r) to (x+r,y+r)
        std::vector<int> mask;
        int r = settings.brushRadius;
        for (int i = x-r; i<x+r;i++) {
            for (int j=y-r; j<y+r; j++) { // check if pixel in mask falls within circle of brush radius
                if (i>=0 && i<m_width && j>=0 && j<m_height) { // stops brush from looping to other side when painting on edge
                    if (pow(x-i,2) + pow(y-j,2) < pow(r,2)) {
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
