#pragma once

#include <QMainWindow>
#include "glwidget.h"
#include <QSlider>
#include <QSpinBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

#include "canvas2d.h"
#include "terrainwindow.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    /// =========== Color Constants ==================
    static constexpr RGBA FOREST_COLOR = RGBA{0,80,5,255};
    static constexpr RGBA GRASSLAND_COLOR = RGBA{0,210,0,255};
    static constexpr RGBA MOUNTAINS_COLOR = RGBA{120,120,120,255};
    static constexpr RGBA DESERT_COLOR = RGBA{240,180,0,255};
    static constexpr RGBA WATER_COLOR = RGBA{0,100,255,255};


private:
    void setupCanvas2D();
    Canvas2D *m_canvas;
    TerrainWindow *m_terrainWindow;


    bool rgbEquals(RGBA x, RGBA y);

    void addHeading(QBoxLayout *layout, QString text);
    void addLabel(QBoxLayout *layout, QString text);
    void addRadioButton(QButtonGroup *group, QBoxLayout *layout, QString text, bool value, auto function);
    void addSpinBox(QBoxLayout *layout, QString text, int min, int max, int step, int val, auto function);
    void addPushButton(QBoxLayout *layout, QString text, auto function);
    void addCheckBox(QBoxLayout *layout, QString text, bool value, auto function);

private slots:
    void setBrushType(int type);
    void setBrushColor(RGBA color);
    void setBoolVal(bool &setValue, bool newValue);
    void setIntVal(int &setValue, int newValue);

    void onClearButtonClick();
    void onUploadButtonClick();
    void onSaveButtonClick();
    void onGenerateTerrainButtonClick();
};
