#include "ShadersInterpolation.h"

#include <QDebug>

ShadersInterpolation::ShadersInterpolation(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

ShadersInterpolation::~ShadersInterpolation()
{
    makeCurrent();

    m_VBO[0].destroy();
    m_VBO[1].destroy();
    m_VAO[0].destroy();
    m_VAO[1].destroy();

    doneCurrent();
}

void ShadersInterpolation::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    //2D triangle
    float vertices1[] = {
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.9f,  0.5f, 0.0f,  // top
    };
    float vertices2[] = {
        0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left
        0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
        0.9f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // top
    };

    //shader初始化
    {
        bool result = true;
        //uniform
        result = m_program[0].addShaderFromSourceFile(QOpenGLShader::Vertex, ":/1.getting_started/3.2shaders_interpolation/shaders_interpolation.vert");
        if(!result){
            qDebug() << m_program[0].log();
        }
        result = m_program[0].addShaderFromSourceFile(QOpenGLShader::Fragment, ":/1.getting_started/3.2shaders_interpolation/shaders_interpolation.frag");
        if(!result){
            qDebug() << m_program[0].log();
        }
        result = m_program[0].link();
        if(!result){
            qDebug() << m_program[0].log();
        }
        //no uniform
        result = m_program[1].addShaderFromSourceFile(QOpenGLShader::Vertex, ":/1.getting_started/3.2shaders_interpolation/shaders_interpolation1.vert");
        if(!result){
            qDebug() << m_program[1].log();
        }
        result = m_program[1].addShaderFromSourceFile(QOpenGLShader::Fragment, ":/1.getting_started/3.2shaders_interpolation/shaders_interpolation1.frag");
        if(!result){
            qDebug() << m_program[1].log();
        }
        result = m_program[1].link();
        if(!result){
            qDebug() << m_program[1].log();
        }
    }
    //
    {
        //1. 绑定顶点数组对象
        m_VAO[0].create();
        m_VAO[0].bind();
        //2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO[0].create();
        m_VBO[0].bind();
        //将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
        m_VBO[0].allocate(vertices1, sizeof(vertices1));
        //3. 设置顶点属性指针
        m_program[0].setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
        m_program[0].enableAttributeArray(0);

        //1. 绑定顶点数组对象
        m_VAO[1].create();
        m_VAO[1].bind();
        //2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO[1].create();
        m_VBO[1].bind();
        //将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
        m_VBO[1].allocate(vertices2, sizeof(vertices2));
        //3. 设置顶点属性指针
        //位置属性
        m_program[1].setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
        m_program[1].enableAttributeArray(0);
        //颜色属性
        m_program[1].setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
        m_program[1].enableAttributeArray(1);
    }

    qsrand(1000);
    m_timer.start(500);
    connect(&m_timer, &QTimer::timeout, this, &ShadersInterpolation::handleTimeout);
}

void ShadersInterpolation::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void ShadersInterpolation::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program[0].bind();
    //更新uniform颜色
    float greenValue = sin(qrand()) / 2.0 + 0.5;
    qDebug() << greenValue;
    m_program[0].setUniformValue("uColor", 0.0, greenValue, 0.0, 1.0);
    m_VAO[0].bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_program[1].bind();
    m_VAO[1].bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_VAO[1].release();
    m_program[1].release();
}

void ShadersInterpolation::handleTimeout()
{
    update();
}

