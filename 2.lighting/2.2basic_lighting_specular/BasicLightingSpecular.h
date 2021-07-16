#ifndef BASICLIGHTINGSPECULAR_H
#define BASICLIGHTINGSPECULAR_H

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

class BasicLightingSpecular : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE BasicLightingSpecular(QWidget *parent = nullptr);
    ~BasicLightingSpecular();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual bool event(QEvent *e) override;

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
};

#endif // BASICLIGHTINGSPECULAR_H
