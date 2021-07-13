#ifndef CAMERACIRCLE_H
#define CAMERACIRCLE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QTimer>
#include <memory>

class CameraCircle : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE CameraCircle(QWidget *parent = nullptr);
    ~CameraCircle();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void handleTimeout();

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    std::vector<QVector3D> m_cubePositions;
    std::vector<QMatrix4x4> m_models;

    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;
    //摄像机位置
    QVector3D m_cameraPos;
    //摄像机方向
    QVector3D m_cameraTarget;
    QVector3D m_cameraDirection;
    //上轴
    QVector3D m_cameraUp;

    QTimer m_timer;
};

#endif // CAMERACIRCLE_H
