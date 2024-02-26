#ifndef HELLOTRIANGLEEXERCISE2_H
#define HELLOTRIANGLEEXERCISE2_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class HelloTriangleExercise2 : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE HelloTriangleExercise2(QWidget *parent = nullptr);
    ~HelloTriangleExercise2();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLVertexArrayObject m_VAO[2];
    QOpenGLBuffer m_VBO[2];

    QOpenGLShaderProgram m_program;
};

#endif // HELLOTRIANGLEEXERCISE2_H
