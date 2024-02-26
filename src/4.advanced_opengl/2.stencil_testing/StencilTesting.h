#ifndef STENCILTESTING_H
#define STENCILTESTING_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <memory>
#include "../../utils/CameraUtil.h"

class StencilTesting : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE StencilTesting(QWidget *parent = nullptr);
    ~StencilTesting();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

private:
    QOpenGLVertexArrayObject m_cubeVAO, m_planeVAO;
    QOpenGLBuffer m_cubeVBO, m_planeVBO;

    QOpenGLShaderProgram m_normalShader;
    QOpenGLShaderProgram m_singleColorShader;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;
};

#endif // STENCILTESTING_H
