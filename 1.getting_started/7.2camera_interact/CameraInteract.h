#ifndef CAMERAINTERACT_H
#define CAMERAINTERACT_H

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
#include <QSet>

class CameraInteract : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE CameraInteract(QWidget *parent = nullptr);
    ~CameraInteract();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void timerEvent(QTimerEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

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
    QVector3D m_cameraDirection;
    //上轴
    QVector3D m_cameraUp;

    //interact
    //key
    QSet<int> m_pressedKeys;
    int m_timerId;
    float m_deltaTime;//当前帧与上一帧的时间差
    float m_lastFrame;//上一帧的时间
    float m_moveSpeed;

    float m_yaw;//偏航角
    float m_pitch;//俯视角
    float m_sensitivity;//鼠标灵敏度

    float m_fov;

    QPoint m_lastMousePos;
};

#endif // CAMERAINTERACT_H
