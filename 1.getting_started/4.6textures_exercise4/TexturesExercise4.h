#ifndef TEXTURESEXERCISE4_H
#define TEXTURESEXERCISE4_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <memory>

class TexturesExercise4 : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit TexturesExercise4(QWidget *parent = nullptr);
    ~TexturesExercise4();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void handleValueChanged(int value);

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    float m_percent;
};

#endif // TEXTURESEXERCISE4_H
