#include "HelloTriangleIndexed.h"

#include <QDebug>

HelloTriangleIndexed::HelloTriangleIndexed(QWidget *parent)
    : QOpenGLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_IBO(QOpenGLBuffer::IndexBuffer)
{

}

HelloTriangleIndexed::~HelloTriangleIndexed()
{
    makeCurrent();

    m_IBO.destroy();
    m_VBO.destroy();
    m_VAO.destroy();

    doneCurrent();
}

void HelloTriangleIndexed::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //shader初始化
    {
        bool result = true;
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/1.getting_started/2.1hello_triangle/hello_triangle.vert");
        if(!result){
            qDebug() << m_program.log();
        }
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/1.getting_started/2.1hello_triangle/hello_triangle.frag");
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
        //2D triangle
        GLfloat vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
        };
        unsigned int indices[] = { // 注意索引从0开始!
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
        };
        //1. 绑定顶点数组对象
        m_VAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
        //2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        //3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
        m_IBO.create();
        m_IBO.bind();
        m_IBO.allocate(indices, sizeof(indices));
        //4. 设置顶点属性指针
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
        m_program.enableAttributeArray(0);
    }
}

void HelloTriangleIndexed::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void HelloTriangleIndexed::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program.bind();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    m_program.release();
}

