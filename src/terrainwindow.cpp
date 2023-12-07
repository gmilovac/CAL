#include "terrainwindow.h"
#include "settings.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QGroupBox>
#include <QTabWidget>
#include <QScrollArea>
#include <QCheckBox>
#include <iostream>

//MainWindow::MainWindow()
//{
//    glWidget = new GLWidget;
//
//    QHBoxLayout *container = new QHBoxLayout;
//    container->addWidget(glWidget);
//    this->setLayout(container);
//}

TerrainWindow::TerrainWindow()
{
    glWidget = new GLWidget();

        QHBoxLayout *container = new QHBoxLayout;
        container->addWidget(glWidget);
        this->setLayout(container);

}
