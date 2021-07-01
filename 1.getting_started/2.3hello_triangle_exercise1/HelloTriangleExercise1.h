#ifndef HELLOTRIANGLEEXERCISE1_H
#define HELLOTRIANGLEEXERCISE1_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class HelloTriangleExercise1 : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit HelloTriangleExercise1(QWidget *parent = nullptr);
    ~HelloTriangleExercise1();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;
    QOpenGLBuffer m_IBO;

    QOpenGLShaderProgram m_program;
};

#endif // HELLOTRIANGLEEXERCISE1_H
