#include "terrainwindow.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <iostream>

TerrainWindow::TerrainWindow()
{
}

void TerrainWindow::newWidget() {
        QHBoxLayout *container = new QHBoxLayout;

        glWidget = new GLWidget();
        container->addWidget(glWidget);

        this->setLayout(container);
}

