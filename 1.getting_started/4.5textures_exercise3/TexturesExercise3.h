#ifndef TEXTURESEXERCISE3_H
#define TEXTURESEXERCISE3_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <memory>

class TexturesExercise3 : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit TexturesExercise3(QWidget *parent = nullptr);
    ~TexturesExercise3();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[2];
};

#endif // TEXTURESEXERCISE3_H
