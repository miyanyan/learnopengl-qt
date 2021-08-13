#ifndef FRAMEBUFFERSEXERCISE1_H
#define FRAMEBUFFERSEXERCISE1_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <memory>
#include "../../utils/CameraUtil.h"

class FramebuffersExercise1 : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE FramebuffersExercise1(QWidget *parent = nullptr);
    ~FramebuffersExercise1();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

private:
    void handleLineMode(bool flag);

private:
    QOpenGLVertexArrayObject m_cubeVAO, m_planeVAO, m_quadVAO;
    QOpenGLBuffer m_cubeVBO, m_planeVBO, m_quadVBO;

    QOpenGLShaderProgram m_shader;
    QOpenGLShaderProgram m_screenShader;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    std::unique_ptr<QOpenGLFramebufferObject> m_FBO;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;

    bool m_isLineMode;
    int m_postProcessingId;
};

#endif // FRAMEBUFFERSEXERCISE1_H
