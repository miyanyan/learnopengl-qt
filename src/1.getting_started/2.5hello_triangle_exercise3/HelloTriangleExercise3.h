#ifndef HELLOTRIANGLEEXERCISE3_H
#define HELLOTRIANGLEEXERCISE3_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class HelloTriangleExercise3 : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE HelloTriangleExercise3(QWidget *parent = nullptr);
    ~HelloTriangleExercise3();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLVertexArrayObject m_VAO[2];
    QOpenGLBuffer m_VBO[2];

    QOpenGLShaderProgram m_program[2];
};

#endif // HELLOTRIANGLEEXERCISE3_H
