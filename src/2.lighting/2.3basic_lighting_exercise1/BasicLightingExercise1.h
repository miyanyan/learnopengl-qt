#ifndef BASICLIGHTINGEXERCISE1_H
#define BASICLIGHTINGEXERCISE1_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include "../../utils/CameraUtil.h"

class BasicLightingExercise1 : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE BasicLightingExercise1(QWidget *parent = nullptr);
    ~BasicLightingExercise1();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual bool event(QEvent *e) override;

private:
    void handleTimeout();

private:
    QOpenGLVertexArrayObject m_lightVAO;
    QOpenGLVertexArrayObject m_lightCubeVAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_lightShader;
    QOpenGLShaderProgram m_lightCubeShader;

    QVector3D m_lightPos;

    QMatrix4x4 m_projection;

    CameraUtil m_camera;

    QSlider* m_sliderAmbientStrength;
    QSlider* m_sliderSpecularStrength;
    QSlider* m_sliderSpecularShininess;

    QTimer m_timer;
};

#endif // BASICLIGHTINGEXERCISE1_H
