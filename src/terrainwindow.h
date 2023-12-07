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


class TerrainWindow : public QWidget
{
    Q_OBJECT


public:
    TerrainWindow();
    GLWidget *glWidget;


private:

};



