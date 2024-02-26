#include "HelloTriangleExercise3.h"

#include <QDebug>

HelloTriangleExercise3::HelloTriangleExercise3(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

HelloTriangleExercise3::~HelloTriangleExercise3()
{
    makeCurrent();

    m_VBO[0].destroy();
    m_VBO[1].destroy();
    m_VAO[0].destroy();
    m_VAO[1].destroy();

    doneCurrent();
}

void HelloTriangleExercise3::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    //2D triangle
    float vertices[2][9] = {
        {
            -0.9f, -0.5f, 0.0f,  // left
            -0.0f, -0.5f, 0.0f,  // right
            -0.9f, 0.5f, 0.0f,  // top
        },
        {
            0.0f, -0.5f, 0.0f,  // left
            0.9f, -0.5f, 0.0f,  // right
            0.9f, 0.5f, 0.0f   // top
        }
    };

    for(int i = 0; i < 2; ++i){
        //shader初始化
        bool result = true;
        result = m_program[i].addShaderFromSourceFile(QOpenGLShader::Vertex, ":/1.getting_started/2.1hello_triangle/hello_triangle.vert");
        if(!result){
            qDebug() << m_program[i].log();
        }
        if(i == 0){
            result = m_program[i].addShaderFromSourceFile(QOpenGLShader::Fragment, ":/1.getting_started/2.1hello_triangle/hello_triangle.frag");
        }else{
            result = m_program[i].addShaderFromSourceFile(QOpenGLShader::Fragment, ":/1.getting_started/2.1hello_triangle/hello_triangle1.frag");
        }

        if(!result){
            qDebug() << m_program[i].log();
        }
        result = m_program[i].link();
        if(!result){
            qDebug() << m_program[i].log();
        }

        //1. 绑定顶点数组对象
        m_VAO[i].create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO[i]);
        //2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO[i].create();
        m_VBO[i].bind();
        m_VBO[i].allocate(vertices[i], sizeof(vertices[i]));
        //3. 设置顶点属性指针
        m_program[i].setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
        m_program[i].enableAttributeArray(0);
    }
}

void HelloTriangleExercise3::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void HelloTriangleExercise3::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program[0].bind();
    m_VAO[0].bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_program[1].bind();
    m_VAO[1].bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_VAO[1].release();
    m_program[1].release();
}

