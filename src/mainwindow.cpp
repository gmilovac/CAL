#include "mainwindow.h"
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
#include <QButtonGroup>



MainWindow::~MainWindow() {}

MainWindow::MainWindow()
{
    setWindowTitle("Terrain Painter");

    // loads in settings from last run or uses default values
    settings.loadSettingsOrDefaults();

    QHBoxLayout *hLayout = new QHBoxLayout(); // horizontal layout for canvas and controls panel
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical layout for control panel

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
    QWidget *brushGroup = new QWidget();
    QVBoxLayout *brushLayout = new QVBoxLayout();
    brushLayout->setAlignment(Qt::AlignTop);
    brushGroup->setLayout(brushLayout);

    QWidget *filterGroup = new QWidget();
    QVBoxLayout *filterLayout = new QVBoxLayout();
    filterLayout->setAlignment(Qt::AlignTop);
    filterGroup->setLayout(filterLayout);

    QScrollArea *controlsScroll = new QScrollArea();
    QTabWidget *tabs = new QTabWidget();
    controlsScroll->setWidget(tabs);
    controlsScroll->setWidgetResizable(true);

    tabs->addTab(brushGroup, "      Terrain Painter     ");
    //tabs->addTab(filterGroup, "Filter");

    vLayout->addWidget(controlsScroll);

    // brush selection
    addHeading(brushLayout, "Brush");
    QButtonGroup *brushButtons = new QButtonGroup();
    QButtonGroup *biomeColors = new QButtonGroup();

    addRadioButton(brushButtons, brushLayout, "Constant", settings.brushType == BRUSH_CONSTANT, [this]{ setBrushType(BRUSH_CONSTANT); });
    //addRadioButton(brushButtons, brushLayout, "Linear", settings.brushType == BRUSH_LINEAR, [this]{ setBrushType(BRUSH_LINEAR); });
    //addRadioButton(brushButtons, brushLayout, "Quadratic", settings.brushType == BRUSH_QUADRATIC, [this]{ setBrushType(BRUSH_QUADRATIC); });
    addRadioButton(brushButtons, brushLayout, "Fill", settings.brushType == BRUSH_FILL, [this]{setBrushType(BRUSH_FILL); });
    addSpinBox(brushLayout, "radius", 0, 100, 1, settings.brushRadius, [this](int value){ setIntVal(settings.brushRadius, value); });
    //addRadioButton(brushLayout, "Smudge", settings.brushType == BRUSH_SMUDGE, [this]{ setBrushType(BRUSH_SMUDGE); });

    addHeading(brushLayout, "Biomes");
    // Biome colors:

    addRadioButton(biomeColors, brushLayout, "Forest", rgbEquals(settings.brushColor,FOREST_COLOR), [this]{setBrushColor(FOREST_COLOR); });
    addRadioButton(biomeColors, brushLayout, "Grassland", rgbEquals(settings.brushColor,GRASSLAND_COLOR), [this]{setBrushColor(GRASSLAND_COLOR); });
    addRadioButton(biomeColors, brushLayout, "Mountain", rgbEquals(settings.brushColor,MOUNTAINS_COLOR), [this]{setBrushColor(MOUNTAINS_COLOR); });
    addRadioButton(biomeColors, brushLayout, "Desert", rgbEquals(settings.brushColor,DESERT_COLOR), [this]{setBrushColor(DESERT_COLOR); });
    addRadioButton(biomeColors, brushLayout, "Ocean", rgbEquals(settings.brushColor,OCEAN_COLOR), [this]{setBrushColor(OCEAN_COLOR); });
    addRadioButton(biomeColors, brushLayout, "Lake", rgbEquals(settings.brushColor,LAKE_COLOR), [this]{setBrushColor(LAKE_COLOR); });
    // brush parameters
    //addSpinBox(brushLayout, "red", 0, 255, 1, settings.brushColor.r, [this](int value){ setUIntVal(settings.brushColor.r, value); });
    //addSpinBox(brushLayout, "green", 0, 255, 1, settings.brushColor.g, [this](int value){ setUIntVal(settings.brushColor.g, value); });
    //addSpinBox(brushLayout, "blue", 0, 255, 1, settings.brushColor.b, [this](int value){ setUIntVal(settings.brushColor.b, value); });
    //addSpinBox(brushLayout, "alpha", 0, 255, 1, settings.brushColor.a, [this](int value){ setUIntVal(settings.brushColor.a, value); });


    addPushButton(brushLayout, "Generate Terrain", &MainWindow::onGenerateTerrainButtonClick);

    addPushButton(brushLayout, "Clear canvas", &MainWindow::onClearButtonClick);

    addPushButton(brushLayout, "Save Image", &MainWindow::onSaveButtonClick);

    addPushButton(brushLayout, "Load Image", &MainWindow::onUploadButtonClick);

    //addPushButton(brushLayout, "Show terrain", &MainWindow::onFilterButtonClick);

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
    //button->setAutoExclusive(false);
    button->setChecked(value);
    //button->setAutoExclusive(true);
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

void MainWindow::addDoubleSpinBox(QBoxLayout *layout, QString text, double min, double max, double step, double val, int decimal, auto function) {
    QDoubleSpinBox *box = new QDoubleSpinBox();
    box->setMinimum(min);
    box->setMaximum(max);
    box->setSingleStep(step);
    box->setValue(val);
    box->setDecimals(decimal);
    QHBoxLayout *subLayout = new QHBoxLayout();
    addLabel(subLayout, text);
    subLayout->addWidget(box);
    layout->addLayout(subLayout);
    connect(box, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, function);
}

void MainWindow::addPushButton(QBoxLayout *layout, QString text, auto function) {
    QPushButton *button = new QPushButton(text);
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, function);
}

void MainWindow::addCheckBox(QBoxLayout *layout, QString text, bool val, auto function) {
    QCheckBox *box = new QCheckBox(text);
    box->setChecked(val);
    layout->addWidget(box);
    connect(box, &QCheckBox::clicked, this, function);
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


void MainWindow::setFilterType(int type) {
    settings.filterType = type;
    m_canvas->settingsChanged();
}

void MainWindow::setUIntVal(std::uint8_t &setValue, int newValue) {
    setValue = newValue;
    m_canvas->settingsChanged();
}

void MainWindow::setIntVal(int &setValue, int newValue) {
    setValue = newValue;
    m_canvas->settingsChanged();
}

void MainWindow::setFloatVal(float &setValue, float newValue) {
    setValue = newValue;
    m_canvas->settingsChanged();
}

void MainWindow::setBoolVal(bool &setValue, bool newValue) {
    setValue = newValue;
    m_canvas->settingsChanged();
}


// ------ PUSH BUTTON FUNCTIONS ------

void MainWindow::onGenerateTerrainButtonClick() {
   // glWidget->m_terrain.generateTerrain(false);
    std::vector<glm::vec4> a = m_canvas->getCanvasData();

    //glWidget->m_col = glm::vec4(1.f,1.f,0.f,1.f);
    // t = new TerrainWindow();
    //m_terrainWindow.TerrainWindow();
    //TerrainWindow t = new TerrainWindow();
    //delete &m_terrainWindow;


   // m_terrainWindow = t;
    m_terrainWindow = new TerrainWindow();
    m_terrainWindow->newWidget();
    m_terrainWindow->glWidget->renderTerrain(a);
    m_terrainWindow->resize(800,600);
    m_terrainWindow->show();
}

void MainWindow::onClearButtonClick() {
    //m_canvas->resize(m_canvas->parentWidget()->size().width(), m_canvas->parentWidget()->size().height());
    m_canvas->clearCanvas();

}

void MainWindow::onFilterButtonClick() {
    m_terrainWindow->resize(800,600);
    m_terrainWindow->show();
}

void MainWindow::onRevertButtonClick() {
    m_canvas->loadImageFromFile(settings.imagePath);
}

void MainWindow::onUploadButtonClick() {
    // Get new image path selected by user
    QString file = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.jpeg)"));
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

