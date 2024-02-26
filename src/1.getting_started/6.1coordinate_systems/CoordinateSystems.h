#ifndef COORDINATESYSTEMS_H
#define COORDINATESYSTEMS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QWheelEvent>
#include <QMouseEvent>
#include <memory>

class CoordinateSystems : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE CoordinateSystems(QWidget *parent = nullptr);
    ~CoordinateSystems();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    float m_scale;
};

#endif // COORDINATESYSTEMS_H
