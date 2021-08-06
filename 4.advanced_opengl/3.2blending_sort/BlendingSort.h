#ifndef BLENDINGSORT_H
#define BLENDINGSORT_H

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

class BlendingSort : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE BlendingSort(QWidget *parent = nullptr);
    ~BlendingSort();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

private:
    QOpenGLVertexArrayObject m_cubeVAO, m_planeVAO, m_transparentVAO;
    QOpenGLBuffer m_cubeVBO, m_planeVBO, m_transparentVBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[3];

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    std::vector<QVector3D> m_windowPos;

    CameraUtil m_camera;
};

#endif // BLENDINGSORT_H
