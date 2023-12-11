#include "terrainwindow.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <iostream>

TerrainWindow::TerrainWindow()
{
}

void TerrainWindow::newWidget(bool cell) {
        QHBoxLayout *container = new QHBoxLayout;

        glWidget = new GLWidget();
        glWidget->setCell(cell);
        container->addWidget(glWidget);
        this->setLayout(container);
}

