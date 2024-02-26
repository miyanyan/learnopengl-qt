#ifndef LIGHTINGMAPSEXERCISE4_H
#define LIGHTINGMAPSEXERCISE4_H

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

class LightingMapsExercise4 : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE LightingMapsExercise4(QWidget *parent = nullptr);
    ~LightingMapsExercise4();
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

    CameraUtil m_camera;

    QTimer m_timer;
};

#endif // LIGHTINGMAPSEXERCISE4_H
