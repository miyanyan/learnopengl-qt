#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include "../../utils/CameraUtil.h"

class CameraClass : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE CameraClass(QWidget *parent = nullptr);
    ~CameraClass();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual bool event(QEvent *e) override;

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    std::vector<QVector3D> m_cubePositions;
    std::vector<QMatrix4x4> m_models;

    QMatrix4x4 m_projection;

    CameraUtil m_camera;
};

#endif // CAMERACLASS_H
