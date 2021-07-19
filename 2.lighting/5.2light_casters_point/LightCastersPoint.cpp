#include "LightCastersPoint.h"

#include <QDebug>
#include <QTime>

LightCastersPoint::LightCastersPoint(QWidget *parent)
    : QOpenGLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_lightPos(1.2, 1.0, 2.0),
      m_camera(this)
{
    QStringList text;
    text << "light constant" << "light linear" << "light quadratic";
    for(int i = 0; i < text.size(); ++i){
        QLabel* label = new QLabel(text[i], this);
        label->setStyleSheet("QLabel { color : white; }");
        label->setGeometry(10, 20 * i, 120, 20);
    }
    m_sliderLightConstant = new QSlider(Qt::Horizontal, this);
    m_sliderLightLinear = new QSlider(Qt::Horizontal, this);
    m_sliderLightQuadratic = new QSlider(Qt::Horizontal, this);

    m_sliderLightConstant->setGeometry(140, 0, 80, 20);
    m_sliderLightLinear->setGeometry(140, 20, 80, 20);
    m_sliderLightQuadratic->setGeometry(140, 40, 80, 20);

    m_sliderLightConstant->setMaximum(1000);
    m_sliderLightLinear->setMaximum(1000);
    m_sliderLightQuadratic->setMaximum(1000);

    m_sliderLightConstant->setMinimum(1);
    m_sliderLightLinear->setMinimum(1);
    m_sliderLightQuadratic->setMinimum(1);

    m_timer.start(10);
    connect(&m_timer, &QTimer::timeout, this, &LightCastersPoint::handleTimeout);
}

LightCastersPoint::~LightCastersPoint()
{
    makeCurrent();

    m_VBO.destroy();
    m_lightVAO.destroy();
    m_lightCubeVAO.destroy();
    m_textureDiffuse->destroy();
    m_textureSpecular->destroy();
    m_textureEmission->destroy();

    doneCurrent();
}

void LightCastersPoint::initializeGL()
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
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/2.lighting/5.2light_casters_point/light_casters_point.vert");
        if(!result){
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/2.lighting/5.2light_casters_point/light_casters_point.frag");
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
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };
        //cube positions
        m_cubePositions = {
            {0.0, 0.0, 0.0},
            {2.0, 5.0, -15.0},
            {-1.5, -2.2, -2.5},
            {-3.8, -2.0, -12.3},
            {2.4, -0.4, -3.5},
            {-1.7, 3.0, -7.5},
            {1.3, -2.0, -2.5},
            {1.5, 2.0, -2.5},
            {1.5, 0.2, -1.5},
            {-1.3, 1.0, -1.5}
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
        //3.1 位置
        m_lightShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
        m_lightShader.enableAttributeArray(0);
        //3.2 法线
        m_lightShader.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 8);
        m_lightShader.enableAttributeArray(1);
        //3.3 纹理
        m_lightShader.setAttributeBuffer(2, GL_FLOAT, sizeof(GLfloat) * 6, 2, sizeof(GLfloat) * 8);
        m_lightShader.enableAttributeArray(2);

        // light cube
        m_lightCubeVAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder1(&m_lightCubeVAO);
        m_VBO.bind();
        m_lightCubeShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
        m_lightCubeShader.enableAttributeArray(0);
    }
    //texture
    {
        m_textureDiffuse = std::make_unique<QOpenGLTexture>(QImage(":/2.lighting/4.1lighting_maps_diffuse_map/container2.png").mirrored());
        m_textureDiffuse->create();
        //纹理环绕方式
        m_textureDiffuse->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_textureDiffuse->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        //纹理过滤
        m_textureDiffuse->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
        //多级渐远纹理
        m_textureDiffuse->generateMipMaps();
        m_textureDiffuse->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

        m_textureSpecular = std::make_unique<QOpenGLTexture>(QImage(":/2.lighting/4.2lighting_maps_specular_map/container2_specular.png").mirrored());
        m_textureSpecular->create();
        //纹理环绕方式
        m_textureSpecular->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_textureSpecular->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        //纹理过滤
        m_textureSpecular->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
        //多级渐远纹理
        m_textureSpecular->generateMipMaps();
        m_textureSpecular->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

        m_textureEmission = std::make_unique<QOpenGLTexture>(QImage(":/2.lighting/4.4lighting_maps_exercise4/matrix.jpg").mirrored());
        m_textureEmission->create();
        //纹理环绕方式
        m_textureEmission->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_textureEmission->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        //纹理过滤
        m_textureEmission->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
        //多级渐远纹理
        m_textureEmission->generateMipMaps();
        m_textureEmission->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    }
    //MVP
    {
        m_models.resize(m_cubePositions.size());
        for(int i = 0; i < m_cubePositions.size(); ++i){
            m_models[i].translate(m_cubePositions[i]);
        }
        m_projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    }
    //相机类初始化
    m_camera.init();
}

void LightCastersPoint::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void LightCastersPoint::paintGL()
{    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //
    m_lightShader.bind();
    //texture
    m_textureDiffuse->bind(0);
    m_lightShader.setUniformValue("material.diffuse", 0);
    m_textureSpecular->bind(1);
    m_lightShader.setUniformValue("material.specular", 1);
    m_textureEmission->bind(2);
    m_lightShader.setUniformValue("material.emission", 2);
    //MVP
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    m_lightShader.setUniformValue("projection", m_projection);
    m_lightShader.setUniformValue("view", m_camera.getViewMatrix());
    m_lightShader.setUniformValue("model", QMatrix4x4());
    //pos
    m_lightShader.setUniformValue("viewPos", m_camera.getCameraPos());
    //material
    m_lightShader.setUniformValue("material.shininess", GLfloat(64.0));
    //light
    m_lightShader.setUniformValue("light.position", m_lightPos);
    m_lightShader.setUniformValue("light.ambient", QVector3D(0.2, 0.2, 0.2));
    m_lightShader.setUniformValue("light.diffuse", QVector3D(0.5, 0.5, 0.5));
    m_lightShader.setUniformValue("light.specular", QVector3D(1.0, 1.0, 1.0));
    m_lightShader.setUniformValue("light.constant", GLfloat(m_sliderLightConstant->value()));
    m_lightShader.setUniformValue("light.linear", GLfloat(m_sliderLightLinear->value() / 1000.0));
    m_lightShader.setUniformValue("light.quadratic", GLfloat(m_sliderLightQuadratic->value() / 1000.0));
    //emission
    m_lightShader.setUniformValue("matrixMove", GLfloat(m_matrixMove));
    m_lightShader.setUniformValue("matrixLight", GLfloat(m_matrixLight));

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_lightVAO);
    for(int i = 0; i < m_cubePositions.size(); ++i){
        //m_models[i].rotate(1.0 , {0.5, 1.0, 0.0});
        m_lightShader.setUniformValue("model", m_models[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    }

    m_textureDiffuse->release();
    m_textureSpecular->release();
    m_textureEmission->release();
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

bool LightCastersPoint::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void LightCastersPoint::handleTimeout()
{
    float cur = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
    m_matrixMove = cur;
    m_matrixLight = (1.0 + sin(cur)) / 2 + 0.5;
    update();
}
