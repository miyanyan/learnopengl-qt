#ifndef SHADERSINTERPOLATION_H
#define SHADERSINTERPOLATION_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QTimer>

class ShadersInterpolation : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit ShadersInterpolation(QWidget *parent = nullptr);
    ~ShadersInterpolation();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void handleTimeout();

private:
    QOpenGLVertexArrayObject m_VAO[2];
    QOpenGLBuffer m_VBO[2];

    QOpenGLShaderProgram m_program[2];

    QTimer m_timer;
};

#endif // SHADERSINTERPOLATION_H
