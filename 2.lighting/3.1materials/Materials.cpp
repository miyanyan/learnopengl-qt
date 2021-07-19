#include "Materials.h"

#include <QDebug>
#include <QTime>

Materials::Materials(QWidget *parent)
    : QOpenGLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_lightPos(0.6, 0.0, 5.0),
      m_camera(this)
{
    m_timer.start(18);
    connect(&m_timer, &QTimer::timeout, this, &Materials::handleTimeout);
}

Materials::~Materials()
{
    makeCurrent();

    m_VBO.destroy();
    m_lightVAO.destroy();

    doneCurrent();
}

void Materials::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    //shader初始化
    {
        bool result = true;
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/2.lighting/3.1materials/materials.vert");
        if(!result){
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/2.lighting/3.1materials/materials.frag");
        if(!result){
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.link();
        if(!result){
            qDebug() << m_lightShader.log();
        }

        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/2.lighting/1.colors/light_cube.vert");
        if(!result){
            qDebug() << m_lightCubeShader.log();
        }
        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/2.lighting/1.colors/light_cube.frag");
        if(!result){
            qDebug() << m_lightCubeShader.log();
        }
        result = m_lightCubeShader.link();
        if(!result){
            qDebug() << m_lightCubeShader.log();
        }
    }
    //
    {
        //2D rect
        GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        // light
        //1. 绑定顶点数组对象
        m_lightVAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_lightVAO);
        //2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        //3. 设置顶点属性指针
        m_lightShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
        m_lightShader.enableAttributeArray(0);
        m_lightShader.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
        m_lightShader.enableAttributeArray(1);

        // light cube
        m_lightCubeVAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder1(&m_lightCubeVAO);
        m_VBO.bind();
        m_lightCubeShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
        m_lightCubeShader.enableAttributeArray(0);
    }
    //MVP
    {
        m_projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    }
    //相机类初始化
    m_camera.init();
    m_camera.setCameraPos({0.0, 0.0, 10.0});
//    m_camera.setYaw(-91.78);
//    m_camera.setPitch(-0.37);
}

void Materials::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Materials::paintGL()
{    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //light
    m_lightShader.bind();
    //MVP
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    m_lightShader.setUniformValue("projection", m_projection);
    m_lightShader.setUniformValue("view", m_camera.getViewMatrix());
    m_lightShader.setUniformValue("model", QMatrix4x4());
    //pos
    m_lightShader.setUniformValue("viewPos", m_camera.getCameraPos());
    //material(silver)
    m_lightShader.setUniformValue("material.ambient", QVector3D(0.19225,0.19225,0.19225));
    m_lightShader.setUniformValue("material.diffuse", QVector3D(0.50754 ,0.50754, 0.50754));
    m_lightShader.setUniformValue("material.specular", QVector3D(0.508273, 0.508273 ,0.508273));
    m_lightShader.setUniformValue("material.shininess", GLfloat(0.4 * 128));
    //light
    m_lightShader.setUniformValue("light.position", m_lightPos);
    m_lightShader.setUniformValue("light.ambient", m_ambientColor);
    m_lightShader.setUniformValue("light.diffuse", m_diffuseColor);
    m_lightShader.setUniformValue("light.specular", QVector3D(1.0, 1.0, 1.0));

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

    m_lightShader.release();

    //lamp object
    m_lightCubeShader.bind();
    //MVP
    m_lightCubeShader.setUniformValue("projection", m_projection);
    m_lightCubeShader.setUniformValue("view", m_camera.getViewMatrix());
    QMatrix4x4 model;
    model.translate(m_lightPos);
    model.scale(0.2);
    m_lightCubeShader.setUniformValue("model", model);

    QOpenGLVertexArrayObject::Binder vaoBinder1(&m_lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

    m_lightCubeShader.release();
}

bool Materials::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void Materials::handleTimeout()
{
    float cur = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;

    m_lightColor.setX(sin(cur) * 2.0);
    m_lightColor.setY(sin(cur) * 0.7);
    m_lightColor.setZ(sin(cur) * 1.3);

    m_diffuseColor = m_lightColor * 0.5;
    m_ambientColor = m_diffuseColor * 0.2;
    update();
}
