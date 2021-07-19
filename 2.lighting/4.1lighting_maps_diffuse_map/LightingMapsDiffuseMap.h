#ifndef LIGHTINGMAPSDIFFUSEMAP_H
#define LIGHTINGMAPSDIFFUSEMAP_H

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

class LightingMapsDiffuseMap : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE LightingMapsDiffuseMap(QWidget *parent = nullptr);
    ~LightingMapsDiffuseMap();
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

    std::unique_ptr<QOpenGLTexture> m_texture;

    QVector3D m_lightPos;

    QVector3D m_lightColor;
    QVector3D m_diffuseColor;
    QVector3D m_ambientColor;

    QMatrix4x4 m_projection;

    CameraUtil m_camera;
};

#endif // LIGHTINGMAPSDIFFUSEMAP_H
