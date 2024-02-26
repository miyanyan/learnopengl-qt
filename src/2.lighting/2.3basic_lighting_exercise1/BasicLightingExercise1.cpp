#include "BasicLightingExercise1.h"

#include <QDebug>
#include <QTime>

BasicLightingExercise1::BasicLightingExercise1(QWidget *parent)
    : QOpenGLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_lightPos(0.6, 0.0, 5.0),
      m_camera(this)
{
    QStringList text;
    text << "ambient strength" << "specular strength" << "specular shininess";
    for(int i = 0; i < text.size(); ++i){
        QLabel* label = new QLabel(text[i], this);
        label->setStyleSheet("QLabel { color : white; }");
        label->setGeometry(10, 20 * i, 120, 20);
    }
    m_sliderAmbientStrength = new QSlider(Qt::Horizontal, this);
    m_sliderSpecularStrength = new QSlider(Qt::Horizontal, this);
    m_sliderSpecularShininess = new QSlider(Qt::Horizontal, this);

    m_sliderAmbientStrength->setGeometry(140, 0, 80, 20);
    m_sliderSpecularStrength->setGeometry(140, 20, 80, 20);
    m_sliderSpecularShininess->setGeometry(140, 40, 80, 20);

    m_sliderAmbientStrength->setMaximum(1000);
    m_sliderSpecularStrength->setMaximum(1000);
    m_sliderSpecularShininess->setMaximum(1000);

    m_sliderAmbientStrength->setMinimum(1);
    m_sliderSpecularStrength->setMinimum(1);
    m_sliderSpecularShininess->setMinimum(1);

    connect(m_sliderAmbientStrength, &QSlider::valueChanged, this, static_cast<void (BasicLightingExercise1::*)()>(&BasicLightingExercise1::update));
    connect(m_sliderSpecularStrength, &QSlider::valueChanged, this, static_cast<void (BasicLightingExercise1::*)()>(&BasicLightingExercise1::update));
    connect(m_sliderSpecularShininess, &QSlider::valueChanged, this, static_cast<void (BasicLightingExercise1::*)()>(&BasicLightingExercise1::update));
    m_timer.start(18);
    connect(&m_timer, &QTimer::timeout, this, &BasicLightingExercise1::handleTimeout);
}

BasicLightingExercise1::~BasicLightingExercise1()
{
    makeCurrent();

    m_VBO.destroy();
    m_lightVAO.destroy();

    doneCurrent();
}

void BasicLightingExercise1::initializeGL()
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
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/2.lighting/2.2basic_lighting_specular/basic_lighting.vert");
        if(!result){
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/2.lighting/2.2basic_lighting_specular/basic_lighting.frag");
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

void BasicLightingExercise1::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void BasicLightingExercise1::paintGL()
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
    //color
    m_lightShader.setUniformValue("objectColor", QVector3D(1.0, 0.5, 0.3));
    m_lightShader.setUniformValue("lightColor", QVector3D(1.0, 1.0, 1.0));
    //pos
    m_lightShader.setUniformValue("lightPos", m_lightPos);
    m_lightShader.setUniformValue("viewPos", m_camera.getCameraPos());
    //strength
    m_lightShader.setUniformValue("ambientStrength", (GLfloat)(m_sliderAmbientStrength->value() / 1000.0));
    m_lightShader.setUniformValue("specularStrength", (GLfloat)(m_sliderSpecularStrength->value() / 1000.0));
    m_lightShader.setUniformValue("specularShininess", (GLfloat)(m_sliderSpecularShininess->value() / 1.0));

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

bool BasicLightingExercise1::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void BasicLightingExercise1::handleTimeout()
{
    float cur = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
    m_lightPos.setX(sin(cur));
    m_lightPos.setY(cos(cur));
    update();
}
