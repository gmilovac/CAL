#include "mainwindow.h"
#include "settings.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QTabWidget>
#include <QScrollArea>
#include <iostream>
#include <QButtonGroup>

MainWindow::~MainWindow() {}

MainWindow::MainWindow() {
    setWindowTitle("Terrain Painter");

    // loads in settings from last run or uses default values
    settings.loadSettingsOrDefaults();

    QHBoxLayout * hLayout = new QHBoxLayout(); // horizontal layout for canvas and controls panel
    QVBoxLayout * vLayout = new QVBoxLayout(); // vertical layout for control panel

    vLayout->setAlignment(Qt::AlignTop);

    hLayout->addLayout(vLayout);
    setLayout(hLayout);

    setupCanvas2D();
    resize(800, 600);

    // makes the canvas into a scroll area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(m_canvas);
    scrollArea->setWidgetResizable(true);
    hLayout->addWidget(scrollArea, 1);

    // groupings by project
    QWidget * brushGroup = new QWidget();
    QVBoxLayout * brushLayout = new QVBoxLayout();
    brushLayout->setAlignment(Qt::AlignTop);
    brushGroup->setLayout(brushLayout);

    QScrollArea *controlsScroll = new QScrollArea();
    QTabWidget *tabs = new QTabWidget();
    controlsScroll->setWidget(tabs);
    controlsScroll->setWidgetResizable(true);

    tabs->addTab(brushGroup, "      Terrain Painter      ");

    vLayout->addWidget(controlsScroll);

    // brush selection
    addHeading(brushLayout, "Brush");
    QButtonGroup *brushButtons = new QButtonGroup();
    QButtonGroup *biomeColors = new QButtonGroup();

    addRadioButton(brushButtons, brushLayout, "Constant", settings.brushType == BRUSH_CONSTANT,
                   [this] { setBrushType(BRUSH_CONSTANT); });
    addRadioButton(brushButtons, brushLayout, "Fill", settings.brushType == BRUSH_FILL,
                   [this] { setBrushType(BRUSH_FILL); });
    addSpinBox(brushLayout, "Brush Radius", 0, 100, 1, settings.brushRadius,
               [this](int value) { setIntVal(settings.brushRadius, value); });

    addHeading(brushLayout, "Biomes");
    addRadioButton(biomeColors, brushLayout, "Forest", rgbEquals(settings.brushColor, FOREST_COLOR),
                   [this] { setBrushColor(FOREST_COLOR); });
    addRadioButton(biomeColors, brushLayout, "Grassland", rgbEquals(settings.brushColor, GRASSLAND_COLOR),
                   [this] { setBrushColor(GRASSLAND_COLOR); });
    addRadioButton(biomeColors, brushLayout, "Mountain", rgbEquals(settings.brushColor, MOUNTAINS_COLOR),
                   [this] { setBrushColor(MOUNTAINS_COLOR); });
    addRadioButton(biomeColors, brushLayout, "Desert", rgbEquals(settings.brushColor, DESERT_COLOR),
                   [this] { setBrushColor(DESERT_COLOR); });
    addRadioButton(biomeColors, brushLayout, "Water", rgbEquals(settings.brushColor, WATER_COLOR),
                   [this] { setBrushColor(WATER_COLOR); });

    addSpinBox(brushLayout, "Blur Radius", 0, 100, 1, settings.blurRadius,
               [this](int value) { setIntVal(settings.blurRadius, value); });

    addCheckBox(brushLayout, "Toon Shading", settings.cell, [this](bool value){ setBoolVal(settings.cell, value); });

    addPushButton(brushLayout, "Generate Terrain", &MainWindow::onGenerateTerrainButtonClick);
    addPushButton(brushLayout, "Clear canvas", &MainWindow::onClearButtonClick);
    addPushButton(brushLayout, "Upload Image", &MainWindow::onUploadButtonClick);
    addPushButton(brushLayout, "Save Image", &MainWindow::onSaveButtonClick);
}

/**
 * @brief Sets up Canvas2D
 */
void MainWindow::setupCanvas2D() {
    m_canvas = new Canvas2D();
    m_canvas->init();

    if (!settings.imagePath.isEmpty()) {
        m_canvas->loadImageFromFile(settings.imagePath);
    }
}

// ------ FUNCTIONS FOR ADDING UI COMPONENTS ------

void MainWindow::addHeading(QBoxLayout *layout, QString text) {
    QFont font;
    font.setPointSize(16);
    font.setBold(true);

    QLabel *label = new QLabel(text);
    label->setFont(font);
    layout->addWidget(label);
}

void MainWindow::addLabel(QBoxLayout *layout, QString text) {
    layout->addWidget(new QLabel(text));
}

void MainWindow::addRadioButton(QButtonGroup *group, QBoxLayout *layout, QString text, bool value, auto function) {
    QRadioButton *button = new QRadioButton(text);
    group->addButton(button);
    button->setChecked(value);
    layout->addWidget(button);
    connect(button, &QRadioButton::clicked, this, function);
}

void MainWindow::addSpinBox(QBoxLayout *layout, QString text, int min, int max, int step, int val, auto function) {
    QSpinBox *box = new QSpinBox();
    box->setMinimum(min);
    box->setMaximum(max);
    box->setSingleStep(step);
    box->setValue(val);
    QHBoxLayout *subLayout = new QHBoxLayout();
    addLabel(subLayout, text);
    subLayout->addWidget(box);
    layout->addLayout(subLayout);
    connect(box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, function);
}

void MainWindow::addCheckBox(QBoxLayout *layout, QString text, bool val, auto function) {
    QCheckBox *box = new QCheckBox(text);
    box->setChecked(val);
    layout->addWidget(box);
    connect(box, &QCheckBox::clicked, this, function);
}

void MainWindow::addPushButton(QBoxLayout *layout, QString text, auto function) {
    QPushButton *button = new QPushButton(text);
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, function);
}

// ------ FUNCTIONS FOR UPDATING SETTINGS ------

void MainWindow::setBrushType(int type) {
    settings.brushType = type;
    m_canvas->settingsChanged();
}

void MainWindow::setBrushColor(RGBA color) {
    settings.brushColor = color;
    m_canvas->settingsChanged();
}

void MainWindow::setBoolVal(bool &setValue, bool newValue) {
    setValue = newValue;
    m_canvas->settingsChanged();
}

void MainWindow::setIntVal(int &setValue, int newValue) {
    setValue = newValue;
    m_canvas->settingsChanged();
}

// ------ PUSH BUTTON FUNCTIONS ------

void MainWindow::onGenerateTerrainButtonClick() {
    std::vector<glm::vec4> canvasData = m_canvas->getCanvasData();
    std::vector<float> noiseData = m_canvas->blurImage(m_canvas->getNoiseMap());
    std::vector<float> heightData = m_canvas->blurImage(m_canvas->getHeightMap());
    m_terrainWindow = new TerrainWindow();
    m_terrainWindow->newWidget(settings.cell);
    m_terrainWindow->glWidget->renderTerrain(canvasData,noiseData,heightData);
    m_terrainWindow->resize(800,600);
    m_terrainWindow->show();
}

void MainWindow::onClearButtonClick() {
    m_canvas->clearCanvas();
}

void MainWindow::onUploadButtonClick() {
    // Get new image path selected by user
    QString file = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.jpeg)"));
    if (file.isEmpty()) { return; }
    settings.imagePath = file;

    // Display new image
    m_canvas->loadImageFromFile(settings.imagePath);
    m_canvas->settingsChanged();
}

bool MainWindow::rgbEquals(RGBA x, RGBA y) {
    if (x.r == y.r) {
        if (x.g == y.g) {
            if (x.b == y.b) {
                return true;
            }
        }
    }
    return false;
}

void MainWindow::onSaveButtonClick() {
    // Get new image path selected by user
    QString file = QFileDialog::getSaveFileName(this, tr("Save Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.jpeg)"));
    if (file.isEmpty()) { return; }

    // Save image
    m_canvas->saveImageToFile(file);
}

