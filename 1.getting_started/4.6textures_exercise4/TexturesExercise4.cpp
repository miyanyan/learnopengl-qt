#include "TexturesExercise4.h"

#include <QDebug>
#include <QSlider>

TexturesExercise4::TexturesExercise4(QWidget *parent)
    : QOpenGLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_percent(0.0)
{
    QSlider* sliderPercent = new QSlider(Qt::Horizontal, this);
    sliderPercent->setGeometry(0, 0, 360, 20);
    sliderPercent->setMaximum(100);
    connect(sliderPercent, &QSlider::valueChanged, this, &TexturesExercise4::handleValueChanged);
}

TexturesExercise4::~TexturesExercise4()
{
    makeCurrent();

    m_VBO.destroy();
    m_VAO.destroy();
    m_texture[0]->destroy();
    m_texture[1]->destroy();

    doneCurrent();
}

void TexturesExercise4::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    //shader初始化
    {
        bool result = true;
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/1.getting_started/4.6textures_exercise4/textures_exercise4.vert");
        if(!result){
            qDebug() << m_program.log();
        }
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/1.getting_started/4.6textures_exercise4/textures_exercise4.frag");
        if(!result){
            qDebug() << m_program.log();
        }
        result = m_program.link();
        if(!result){
            qDebug() << m_program.log();
        }
    }
    //
    {
        //2D rect
        GLfloat vertices[] = {
            // 位置              // 颜色               //纹理坐标
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,    0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,    0.0f, 1.0f,   // 左上
             0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    1.0f, 1.0f,   // 右上
        };
        //1. 绑定顶点数组对象
        m_VAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
        //2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        //3. 设置顶点属性指针
        //3.1 位置
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
        m_program.enableAttributeArray(0);
        //3.2 颜色
        m_program.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 8);
        m_program.enableAttributeArray(1);
        //3.3 纹理
        m_program.setAttributeBuffer(2, GL_FLOAT, sizeof(GLfloat) * 6, 2, sizeof(GLfloat) * 8);
        m_program.enableAttributeArray(2);
    }
    //texture
    m_texture[0] = std::make_unique<QOpenGLTexture>(QImage(":/1.getting_started/4.1textures/awesomeface.png").mirrored());
    m_texture[1] = std::make_unique<QOpenGLTexture>(QImage(":/1.getting_started/4.1textures/container.jpg").mirrored());
    for(int i = 0; i < 2; ++i){
        m_texture[i]->create();
        //纹理环绕方式
        m_texture[i]->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_texture[i]->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        //纹理过滤
        m_texture[i]->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        //多级渐远纹理
        m_texture[i]->generateMipMaps();
        m_texture[i]->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    }
}

void TexturesExercise4::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void TexturesExercise4::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program.bind();
    m_texture[0]->bind(0);
    m_program.setUniformValue("texture1", 0);
    m_texture[1]->bind(1);
    m_program.setUniformValue("texture2", 1);
    m_program.setUniformValue("percent", m_percent);
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);//GL_POLYGON在3.x版本已弃用
    m_texture[0]->release();
    m_texture[1]->release();
    m_program.release();
}

void TexturesExercise4::handleValueChanged(int value)
{
    m_percent = value / 100.0;
    update();
}

