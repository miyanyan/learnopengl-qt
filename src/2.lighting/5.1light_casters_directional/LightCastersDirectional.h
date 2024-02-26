#ifndef LIGHTCASTERSDIRECTIONAL_H
#define LIGHTCASTERSDIRECTIONAL_H

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

class LightCastersDirectional : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE LightCastersDirectional(QWidget *parent = nullptr);
    ~LightCastersDirectional();
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

    std::unique_ptr<QOpenGLTexture> m_textureDiffuse;
    std::unique_ptr<QOpenGLTexture> m_textureSpecular;
    std::unique_ptr<QOpenGLTexture> m_textureEmission;

    float m_matrixLight;
    float m_matrixMove;

    QVector3D m_lightPos;

    QMatrix4x4 m_projection;

    std::vector<QVector3D> m_cubePositions;
    std::vector<QMatrix4x4> m_models;

    CameraUtil m_camera;

    QTimer m_timer;
};

#endif // LIGHTCASTERSDIRECTIONAL_H
