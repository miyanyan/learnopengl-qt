#include "BlendingDiscard.h"

#include <QDebug>
#include <QMessageBox>

BlendingDiscard::BlendingDiscard(QWidget *parent)
    : QOpenGLWidget(parent),
      m_cubeVBO(QOpenGLBuffer::VertexBuffer),
      m_planeVBO(QOpenGLBuffer::VertexBuffer),
      m_camera(this)
{
}

BlendingDiscard::~BlendingDiscard()
{
    makeCurrent();

    m_texture[0]->destroy();
    m_texture[1]->destroy();
    m_texture[2]->destroy();

    doneCurrent();
}

void BlendingDiscard::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //shader初始化
    {
        bool result = true;
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/4.advanced_opengl/3.1blending_discard/blending_discard.vert");
        if(!result){
            qDebug() << m_program.log();
        }
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/4.advanced_opengl/3.1blending_discard/blending_discard.frag");
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
        // set up vertex data (and buffer(s)) and configure vertex attributes
        GLfloat cubeVertices[] = {
            // 位置              //纹理坐标
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        GLfloat planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
             5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

             5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
             5.0f, -0.5f, -5.0f,  2.0f, 2.0f
        };

        GLfloat transparentVertices[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
        };
        // cube
        m_cubeVAO.create();
        m_cubeVAO.bind();
        m_cubeVBO.create();
        m_cubeVBO.bind();
        m_cubeVBO.allocate(cubeVertices, sizeof(cubeVertices));
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 5);
        m_program.enableAttributeArray(0);
        m_program.setAttributeBuffer(1, GL_FLOAT, sizeof(GL_FLOAT) * 3, 2, sizeof(GL_FLOAT) * 5);
        m_program.enableAttributeArray(1);
        m_cubeVAO.release();
        // plane
        m_planeVAO.create();
        m_planeVAO.bind();
        m_planeVBO.create();
        m_planeVBO.bind();
        m_planeVBO.allocate(planeVertices, sizeof(planeVertices));
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 5);
        m_program.enableAttributeArray(0);
        m_program.setAttributeBuffer(1, GL_FLOAT, sizeof(GL_FLOAT) * 3, 2, sizeof(GL_FLOAT) * 5);
        m_program.enableAttributeArray(1);
        m_planeVAO.release();
        // transparent
        m_transparentVAO.create();
        m_transparentVAO.bind();
        m_transparentVBO.create();
        m_transparentVBO.bind();
        m_transparentVBO.allocate(transparentVertices, sizeof(transparentVertices));
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 5);
        m_program.enableAttributeArray(0);
        m_program.setAttributeBuffer(1, GL_FLOAT, sizeof(GL_FLOAT) * 3, 2, sizeof(GL_FLOAT) * 5);
        m_program.enableAttributeArray(1);
        m_transparentVAO.release();
    }
    //texture
    m_texture[0] = std::make_unique<QOpenGLTexture>(QImage(":/resources/textures/marble.jpg").mirrored());
    m_texture[1] = std::make_unique<QOpenGLTexture>(QImage(":/resources/textures/metal.png").mirrored());
    m_texture[2] = std::make_unique<QOpenGLTexture>(QImage(":/resources/textures/grass.png"));
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
    //MVP
    {
        m_vegetation = {
            QVector3D(-1.5, 0.0, -0.48),
            QVector3D( 1.5, 0.0,  0.51),
            QVector3D( 0.0, 0.0,  0.7),
            QVector3D(-0.3, 0.0, -2.3),
            QVector3D( 0.5, 0.0, -0.6)
        };
        m_view.translate({0.0, 0.0, -3.0});
        m_projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    }
    //相机类初始化
    m_camera.init();
}

void BlendingDiscard::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void BlendingDiscard::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.bind();
    m_program.setUniformValue("texture1", 0);
    // MVP
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    m_program.setUniformValue("projection", m_projection);
    m_program.setUniformValue("view", m_camera.getViewMatrix());

    // cubes
    m_texture[0]->bind();
    m_model.setToIdentity();
    m_model.translate({-1.0f, 0.0f, -1.0f});
    m_program.setUniformValue("model", m_model);
    m_cubeVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_model.setToIdentity();
    m_model.translate({2.0f, 0.0f, 0.0f});
    m_program.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_cubeVAO.release();

    // floor
    m_planeVAO.bind();
    m_texture[1]->bind();
    m_model.setToIdentity();
    m_program.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_planeVAO.release();

    // vegetation
    m_transparentVAO.bind();
    m_texture[2]->bind();
    for (int i = 0; i < m_vegetation.size(); ++i) {
        m_model.setToIdentity();
        m_model.translate(m_vegetation[i]);
        m_program.setUniformValue("model", m_model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    m_texture[0]->release();
    m_texture[1]->release();
    m_texture[2]->release();
    m_program.release();
}

bool BlendingDiscard::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}
