#ifndef COORDINATESYSTEMSEXERCISE_H
#define COORDINATESYSTEMSEXERCISE_H

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
#include <QSlider>
#include <memory>

class CoordinateSystemsExercise : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE CoordinateSystemsExercise(QWidget *parent = nullptr);
    ~CoordinateSystemsExercise();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

private:
    void handleTimeout();
    void handleMatrix();

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    std::vector<QSlider*> m_sliders;

    std::vector<QVector3D> m_cubePositions;
    std::vector<QMatrix4x4> m_models;

    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;

    QTimer m_timer;
};

#endif // COORDINATESYSTEMSEXERCISE_H
