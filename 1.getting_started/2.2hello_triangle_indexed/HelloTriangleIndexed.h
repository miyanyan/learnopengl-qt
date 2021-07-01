#ifndef HELLOTRIANGLEINDEXED_H
#define HELLOTRIANGLEINDEXED_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class HelloTriangleIndexed : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit HelloTriangleIndexed(QWidget *parent = nullptr);
    ~HelloTriangleIndexed();
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

#endif // HELLOTRIANGLEINDEXED_H
