//
// Created by Gordan Milovac on 12/6/23.
//

#ifndef CAL_CANVAS2D_H
#define CAL_CANVAS2D_H

#include "glm/glm.hpp"
#include <QLabel>
#include <QMouseEvent>
#include <array>
#include "rgba.h"
#include <set>

class Canvas2D : public QLabel {
Q_OBJECT
public:
    int m_width = 0;
    int m_height = 0;

    void init();
    void clearCanvas();
    bool loadImageFromFile(const QString &file);
    bool saveImageToFile(const QString &file);
    void displayImage();
    void resize(int w, int h);

    std::vector<glm::vec4> getCanvasData();

    // This will be called when the settings have changed
    void settingsChanged();

    // Filter TODO: implement
    void filterImage();

private:
    std::vector<RGBA> m_data;
    std::vector<RGBA> smudgeMask;
    void fill(RGBA col, int x, int y, int depth);
    void fillY(RGBA col, int x, int y, int depth);
    bool rgbEquals(RGBA a, RGBA b);

    bool m_isDown;
    void editCanvas(int x, int y);
    std::vector<int> getMask(int x, int y);

    //For fix alpha blending below
    std::set<int> visitedPixels; // keeps track of visited pixels for each mouseDown event, allows for constant opacity for a given brush stroke
    void mouseDown(int x, int y);
    void mouseDragged(int x, int y);
    void mouseUp(int x, int y);

    int posToIndex(int x,int y);
    void blendColor(RGBA brushColor, int x, int y, int i);

    void captureSmudge(std::vector<int> index);


    void filterBlur(bool isGaussian);
    void filterScale(float scaleX, float scaleY);
    void filterEdge();
    void filterGray();
    void filterMedian();
    void filterChroma();

    RGBA hPrimeX(int k, double a, int row);
    RGBA hPrimeY(int k, double a, int col);


    int indexToX(int index);
    int indexToY(int index);
    float getMaskAlpha(int x, int y, int x2, int y2);

    /**
     * lab 3
     */
    void applyFilter();

    // These are functions overriden from QWidget that we've provided
    // to prevent you from having to interact with Qt's mouse events.
    // These will pass the mouse coordinates to the above mouse functions
    // that you will have to fill in.
    virtual void mousePressEvent(QMouseEvent* event) override {
        auto [x, y] = std::array{ event->position().x(), event->position().y() };
        mouseDown(static_cast<int>(x), static_cast<int>(y));
    }
    virtual void mouseMoveEvent(QMouseEvent* event) override {
        auto [x, y] = std::array{ event->position().x(), event->position().y() };
        mouseDragged(static_cast<int>(x), static_cast<int>(y));
    }
    virtual void mouseReleaseEvent(QMouseEvent* event) override {
        auto [x, y] = std::array{ event->position().x(), event->position().y() };
        mouseUp(static_cast<int>(x), static_cast<int>(y));
    }


    // TODO: add any member variables or functions you need
};


#endif //CAL_CANVAS2D_H
