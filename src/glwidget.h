#pragma once

#include "GL/glew.h"
#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "terraingenerator.h"
#include <QMatrix4x4>
#include <QElapsedTimer>
#include "rgba.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget
{
public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();
    void renderTerrain(std::vector<glm::vec4> canvas, std::vector<float> noise, std::vector<float> height);
    glm::vec4 m_col = glm::vec4(1.f,0.f,0.f,1.f);
    void setCell(bool cell);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void rebuildMatrices();

    QElapsedTimer m_elapsedTimer;
    int m_timer;

    // ==== Canvas Variables =======
    std::vector<glm::vec4> m_canvasData;
    std::vector<float> m_noiseData;
    std::vector<float> m_heightData;
    int m_canvasWidth;
    int m_canvasHeight;
    // =============================

    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;
    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLVertexArrayObject m_terrainVao;
    QOpenGLBuffer m_terrainVbo;
    QMatrix4x4 m_proj;
    QVector3D m_cameraPos;
    QVector3D m_cameraUp;
    QVector3D m_cameraLook;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    TerrainGenerator m_terrain;
    bool m_cell;

//    BUMP MAP ATTEMPT
//    GLuint m_rock_texture;
//    GLuint m_snow_texture;
//    GLuint m_forest_texture;
//    GLuint m_grass_texture;
//    GLuint m_sand_texture;
//    GLuint m_ocean_texture;

    int m_projMatrixLoc = 0;
    int m_mvMatrixLoc = 0;

    std::unordered_map<Qt::Key, bool> m_keyMap;
    QPoint m_prevMousePos;
    float m_angleX;
    float m_angleY;
    float m_zoom;
};
