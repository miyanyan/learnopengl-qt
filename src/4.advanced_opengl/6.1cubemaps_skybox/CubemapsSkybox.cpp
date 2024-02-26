#include "CubemapsSkybox.h"

#include <QDebug>
#include <QMessageBox>

CubemapsSkybox::CubemapsSkybox(QWidget *parent)
    : QOpenGLWidget(parent),
      m_cubeVBO(QOpenGLBuffer::VertexBuffer),
      m_skyboxVBO(QOpenGLBuffer::VertexBuffer),
      m_cubeTexture(QOpenGLTexture::Target2D),
      m_camera(this)
{
}

CubemapsSkybox::~CubemapsSkybox()
{
    makeCurrent();

    m_cubemapTexture->destroy();
    m_cubeTexture.destroy();

    doneCurrent();
}

void CubemapsSkybox::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    //shader初始化
    m_shader = std::make_unique<ShaderUtil>(":/4.advanced_opengl/6.1cubemaps_skybox/cubemaps.vert",
                                            ":/4.advanced_opengl/6.1cubemaps_skybox/cubemaps.frag");
    m_skyboxShader = std::make_unique<ShaderUtil>(":/4.advanced_opengl/6.1cubemaps_skybox/skybox.vert",
                                                  ":/4.advanced_opengl/6.1cubemaps_skybox/skybox.frag");
    //
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        GLfloat cubeVertices[] = {
            // positions          // texture Coords
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
        float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        m_cubeVAO.create();
        m_cubeVAO.bind();
        m_cubeVBO.create();
        m_cubeVBO.bind();
        m_cubeVBO.allocate(cubeVertices, sizeof(cubeVertices));
        m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 5);
        m_shader->enableAttributeArray(0);
        m_shader->setAttributeBuffer(1, GL_FLOAT, sizeof(GL_FLOAT) * 3, 2, sizeof(GL_FLOAT) * 5);
        m_shader->enableAttributeArray(1);
        m_cubeVAO.release();

        m_skyboxVAO.create();
        m_skyboxVAO.bind();
        m_skyboxVBO.create();
        m_skyboxVBO.bind();
        m_skyboxVBO.allocate(skyboxVertices, sizeof(skyboxVertices));
        m_skyboxShader->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 3);
        m_skyboxShader->enableAttributeArray(0);
        m_skyboxVAO.release();
    }
    //texture
    {
        m_cubeTexture.setData(QImage(":/resources/textures/container.jpg"));

        m_cubemapTexture = std::make_unique<CubemapTextureUtil>(":/resources/textures/skybox/right.jpg",
                                                                ":/resources/textures/skybox/top.jpg",
                                                                ":/resources/textures/skybox/front.jpg",
                                                                ":/resources/textures/skybox/left.jpg",
                                                                ":/resources/textures/skybox/bottom.jpg",
                                                                ":/resources/textures/skybox/back.jpg");

        m_cubemapTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
        m_cubemapTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_cubemapTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    }

    //shader configuration
    m_shader->bind();
    m_shader->setUniformValue("texture1", 0);
    m_skyboxShader->bind();
    m_skyboxShader->setUniformValue("skybox", 0);
    //相机类初始化
    m_camera.init();
}

void CubemapsSkybox::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void CubemapsSkybox::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw scene as normal
    m_shader->bind();
    // MVP
    m_model.setToIdentity();
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    m_shader->setUniformValue("projection", m_projection);
    m_shader->setUniformValue("view", m_camera.getViewMatrix());
    m_shader->setUniformValue("model", m_model);
    // cubes
    m_cubeVAO.bind();
    m_cubeTexture.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_cubeVAO.release();

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);
    m_skyboxShader->bind();
    // 去除移动的部分，注意```view(3, 3) = 1.0f```!!
    auto view = m_camera.getViewMatrix();
    view.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    view.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    m_skyboxShader->setUniformValue("view", view);
    m_skyboxShader->setUniformValue("projection", m_projection);

    // skybox cube
    m_skyboxVAO.bind();
    m_cubemapTexture->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

    glDepthFunc(GL_LESS);

    m_cubeTexture.release();
    m_cubemapTexture->release();
    m_skyboxShader->release();
}

bool CubemapsSkybox::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}
