#include "StencilTesting.h"
#include <QDebug>
#include <QMessageBox>

StencilTesting::StencilTesting(QWidget *parent)
    : QOpenGLWidget(parent),
      m_cubeVBO(QOpenGLBuffer::VertexBuffer),
      m_planeVBO(QOpenGLBuffer::VertexBuffer),
      m_camera(this)
{

}

StencilTesting::~StencilTesting()
{
    makeCurrent();

    m_texture[0]->destroy();
    m_texture[1]->destroy();

    doneCurrent();
}

void StencilTesting::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    //shader初始化
    {
        bool result = true;
        result = m_normalShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/4.advanced_opengl/2.stencil_testing/stencil_testing.vert");
        if(!result){
            qDebug() << m_normalShader.log();
        }
        result = m_normalShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/4.advanced_opengl/2.stencil_testing/stencil_testing.frag");
        if(!result){
            qDebug() << m_normalShader.log();
        }
        result = m_normalShader.link();
        if(!result){
            qDebug() << m_normalShader.log();
        }

        result = m_singleColorShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/4.advanced_opengl/2.stencil_testing/stencil_testing.vert");
        if(!result){
            qDebug() << m_singleColorShader.log();
        }
        result = m_singleColorShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/4.advanced_opengl/2.stencil_testing/stencil_single_color.frag");
        if(!result){
            qDebug() << m_singleColorShader.log();
        }
        result = m_singleColorShader.link();
        if(!result){
            qDebug() << m_singleColorShader.log();
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

        m_cubeVAO.create();
        m_cubeVAO.bind();
        m_cubeVBO.create();
        m_cubeVBO.bind();
        m_cubeVBO.allocate(cubeVertices, sizeof(cubeVertices));
        m_normalShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 5);
        m_normalShader.enableAttributeArray(0);
        m_normalShader.setAttributeBuffer(1, GL_FLOAT, sizeof(GL_FLOAT) * 3, 2, sizeof(GL_FLOAT) * 5);
        m_normalShader.enableAttributeArray(1);
        m_cubeVAO.release();

        m_planeVAO.create();
        m_planeVAO.bind();
        m_planeVBO.create();
        m_planeVBO.bind();
        m_planeVBO.allocate(planeVertices, sizeof(planeVertices));
        m_normalShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 5);
        m_normalShader.enableAttributeArray(0);
        m_normalShader.setAttributeBuffer(1, GL_FLOAT, sizeof(GL_FLOAT) * 3, 2, sizeof(GL_FLOAT) * 5);
        m_normalShader.enableAttributeArray(1);
        m_planeVAO.release();
    }
    //texture
    m_texture[0] = std::make_unique<QOpenGLTexture>(QImage(":/resources/textures/marble.jpg").mirrored());
    m_texture[1] = std::make_unique<QOpenGLTexture>(QImage(":/resources/textures/metal.png").mirrored());
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

    //相机类初始化
    m_camera.init();
}

void StencilTesting::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void StencilTesting::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // MVP
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    // two shaders need to bind and set MVP
    m_singleColorShader.bind();
    m_singleColorShader.setUniformValue("projection", m_projection);
    m_singleColorShader.setUniformValue("view", m_camera.getViewMatrix());
    m_normalShader.bind();
    m_normalShader.setUniformValue("projection", m_projection);
    m_normalShader.setUniformValue("view", m_camera.getViewMatrix());

    // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
    glStencilMask(0x00);
    // floor
    m_planeVAO.bind();
    m_texture[1]->bind();
    m_normalShader.setUniformValue("texture1", 0);
    m_model.setToIdentity();
    m_normalShader.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_planeVAO.release();

    // 1st. render pass, draw objects as normal, writing to the stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
    glStencilMask(0xFF); // 启用模板缓冲写入
    // cubes
    m_texture[0]->bind();
    m_normalShader.setUniformValue("texture1", 0);
    m_model.setToIdentity();
    m_model.translate({-1.0f, 0.0f, -1.0f});
    m_normalShader.setUniformValue("model", m_model);
    m_cubeVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_model.setToIdentity();
    m_model.translate({2.0f, 0.0f, 0.0f});
    m_normalShader.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_cubeVAO.release();
    m_normalShader.release();

    // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
    // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
    // the objects' size differences, making it look like borders.
    // -----------------------------------------------------------------------------------------------------------------------------
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST); // 禁用了深度测试，让放大的箱子，即边框，不会被地板所覆盖
    m_singleColorShader.bind();
    float scale = 1.1;
    // cubes
    m_texture[0]->bind();
    m_singleColorShader.setUniformValue("texture1", 0);
    m_model.setToIdentity();
    m_model.translate({-1.0f, 0.0f, -1.0f});
    m_model.scale(scale);
    m_singleColorShader.setUniformValue("model", m_model);
    m_cubeVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_model.setToIdentity();
    m_model.translate({2.0f, 0.0f, 0.0f});
    m_model.scale(scale);
    m_singleColorShader.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_cubeVAO.release();
    m_singleColorShader.release();

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

    m_texture[0]->release();
    m_texture[1]->release();

}

bool StencilTesting::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}
