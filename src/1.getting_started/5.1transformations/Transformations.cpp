#include "Transformations.h"

#include <QDebug>

Transformations::Transformations(QWidget *parent)
    : QOpenGLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_scale(1.0)
{

}

Transformations::~Transformations()
{
    makeCurrent();

    m_VBO.destroy();
    m_VAO.destroy();
    m_texture[0]->destroy();
    m_texture[1]->destroy();

    doneCurrent();
}

void Transformations::initializeGL()
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
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/1.getting_started/5.1transformations/transformations.vert");
        if(!result){
            qDebug() << m_program.log();
        }
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/1.getting_started/5.1transformations/transformations.frag");
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
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,    0.0f, 1.0f,   // 左下
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
    m_texture[0] = std::make_unique<QOpenGLTexture>(QImage(":/resources/textures/awesomeface.png").mirrored());
    m_texture[1] = std::make_unique<QOpenGLTexture>(QImage(":/resources/textures/container.jpg").mirrored());
    for(int i = 0; i < 2; ++i){
        m_texture[i]->create();
        //纹理环绕方式
        m_texture[i]->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_texture[i]->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        //纹理过滤
        m_texture[i]->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
        //多级渐远纹理
        m_texture[i]->generateMipMaps();
        m_texture[i]->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    }
}

void Transformations::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Transformations::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program.bind();
    m_texture[0]->bind(0);
    m_program.setUniformValue("texture1", 0);
    m_texture[1]->bind(1);
    m_program.setUniformValue("texture2", 1);
    //trans
    m_program.setUniformValue("transform", m_transform);
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);//GL_POLYGON在3.x版本已弃用
    m_texture[0]->release();
    m_texture[1]->release();
    m_program.release();
}

void Transformations::mousePressEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
    //右键回到初始位置
    if(event->button() == Qt::RightButton){
        m_transform.setToIdentity();
        update();
    }
}

void Transformations::mouseMoveEvent(QMouseEvent *event)
{
    float dx = event->x() - m_mousePos.x();
    float dy = event->y() - m_mousePos.y();
    qDebug() << "mouse move : (" << dx << ", " << dy << ")";
    if(event->buttons() == Qt::LeftButton){
        m_transform.translate(dx/2000, -dy/2000, 0);
        update();
    }
}

void Transformations::wheelEvent(QWheelEvent *event)
{
    auto degree = event->angleDelta();
    double factor = degree.y() / 360.0;

    factor += 1.0;
    qDebug() << "wheel degree : " << degree.y() << ", " << "scale factor" << factor;

    m_transform.scale(factor);
    update();
}

