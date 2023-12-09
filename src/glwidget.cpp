#include "glwidget.h"
#include "terraingenerator.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions_3_1>
#include <QMouseEvent>
#include <QSurfaceFormat>
#include <QDir>
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_angleX(0), m_angleY(0), m_zoom(1.0)
{
    //setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;
}

GLWidget::~GLWidget() {}

void GLWidget::initializeGL()
{
    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    glClearColor(0.4f, 0.6f, 1.f, 1); // sky color
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/resources/shader/vertex.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/resources/shader/fragment.frag");
    m_program->link();
    m_program->bind();

    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");

    m_terrainVao.create();
    m_terrainVao.bind();
    std::vector<glm::vec4> h(1);

    h[0] = m_col;

    std::vector<GLfloat> verts = m_terrain.generateTerrain(m_canvasData, m_noiseData, m_heightData);

    m_terrainVbo.create();
    m_terrainVbo.bind();
    m_terrainVbo.allocate(verts.data(),verts.size()*sizeof(GLfloat));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                             nullptr);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                             reinterpret_cast<void *>(6 * sizeof(GLfloat)));

    m_terrainVbo.release();

    m_world.setToIdentity();
    m_world.translate(QVector3D(-0.5,-0.5,0));



    m_cameraPos = QVector3D(1.f,1.f,1.f);
    m_cameraLook = QVector3D(0.f,0.f,0.f);
    m_cameraUp = QVector3D(0.f,0.f,1.f);
    //camera = Camera(m_cameraPos,m_cameraLook,m_cameraUp, width() / height(), 45.0f, 100.0f, 0.01f);
    m_camera.lookAt(m_cameraPos,m_cameraLook,m_cameraUp);


    m_program->release();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    m_program->setUniformValue(m_program->uniformLocation("wireshade"),m_terrain.m_wireshade);

    int res = m_terrain.getResolution();

    glPolygonMode(GL_FRONT_AND_BACK,m_terrain.m_wireshade? GL_LINE : GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, res * res * 6);

    m_program->release();
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void GLWidget::timerEvent(QTimerEvent *event) {
//    int elapsedms   = m_elapsedTimer.elapsed();
//    float deltaTime = elapsedms * 0.001f;
//    m_elapsedTimer.restart();
//
//    if (m_keyMap[Qt::Key_W]){
//        std::cout << "W" << std::endl;
//        m_cameraPos = m_cameraPos + deltaTime * m_cameraLook;
//        m_camera.lookAt(m_cameraPos,m_cameraPos + m_cameraLook,m_cameraUp);
//        m_proj.setToIdentity();
//        m_proj.perspective(45.0f, 1.0 * width() / height(), 0.01f, 100.0f);
//    }
//    if (m_keyMap[Qt::Key_S]){
//        std::cout << "S" << std::endl;
//        m_cameraPos = m_cameraPos - deltaTime * m_cameraLook;
//        m_camera.lookAt(m_cameraPos,m_cameraLook,m_cameraUp);
//    }
//    if (m_keyMap[Qt::Key_A]){
//        cross(m_cameraLook,m_cameraUp);
//        m_camera.translate(deltaTime);
//    }
//    if (m_keyMap[Qt::Key_D]){
//        moveCamera(5.f * deltaTime * glm::normalize(glm::vec4(glm::cross(glm::vec3(m_data.cameraData.look), glm::vec3(m_data.cameraData.up)), 1.f)));
//    }
//    if (m_keyMap[Qt::Key_Control]){
//        moveCamera(5.f * deltaTime * glm::vec4(0.f, -1.f, 0.f, 0.f));
//    }
//    if (m_keyMap[Qt::Key_Space]){
//        moveCamera(5.f * deltaTime * glm::vec4(0.f, 1.f, 0.f, 0.f));
//    }
//    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_prevMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    m_angleX += 10 * (event->position().x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 10 * (event->position().y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    m_zoom -= event->angleDelta().y() / 100.f;
    rebuildMatrices();
}

void GLWidget::rebuildMatrices() {
    m_camera.setToIdentity();
    QMatrix4x4 rot;
    rot.setToIdentity();
//    QVector3D axis = QVector3D::crossProduct(m_cameraLook, m_cameraUp);
//    rot.rotate(m_angleX, axis);
//    m_cameraLook = rot * m_cameraLook;
//    m_cameraUp = rot * m_cameraUp;

    rot.rotate(-10 * m_angleX,QVector3D(0,0,1));
    QVector3D eye = m_cameraPos;
    eye = rot.map(eye);
//    QMatrix4x4 rot2;
//    rot.setToIdentity();
//    rot2.rotate(m_angleY, QVector3D(0, 1, 0));
//    m_cameraLook = rot2 * m_cameraLook;
//    m_cameraUp = rot2 * m_cameraUp;

    rot.setToIdentity();
    rot.rotate(-10 * m_angleY,QVector3D::crossProduct(QVector3D(0,1,0),eye));
    eye = rot.map(eye);

    eye = eye * m_zoom;

    m_camera.lookAt(eye,m_cameraLook,m_cameraUp);

    m_proj.setToIdentity();
    m_proj.perspective(45.0f, 1.0 * width() / height(), 0.01f, 100.0f);

    update();
}

void GLWidget::renderTerrain(std::vector<glm::vec4> canvas, std::vector<float> noise, std::vector<float> height) {
    m_canvasData = canvas;
    m_noiseData = noise;
    m_heightData = height;

}
