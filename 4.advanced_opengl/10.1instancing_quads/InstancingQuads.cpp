#include "InstancingQuads.h"

#include <QDebug>

InstancingQuads::InstancingQuads(QWidget *parent)
    : QOpenGLWidget(parent),
      m_camera(this)
{
}

InstancingQuads::~InstancingQuads()
{
    makeCurrent();

    doneCurrent();
}

void InstancingQuads::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize(); // initializes in the current context, i.e. ctx
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &InstancingQuads::handleLoggedMessage);
    logger->startLogging();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //shader初始化
    {
        m_shader = std::make_unique<ShaderUtil>(":/4.advanced_opengl/10.1instancing_quads/instancing.vert",
                                                ":/4.advanced_opengl/10.1instancing_quads/instancing.frag");
    }
    {
        // note : vector<vector<GLfloat>>(100, vector<GLfloat>(2)) is wrong
        //        because vector of vectors are not stored contiguously
        std::vector<GLfloat> translations(100 * 2);
        int index = 0;
        GLfloat offset = 0.1f;
        for (int i = -10; i < 10; i += 2) {
            for (int j = -10; j < 10; j += 2) {
                translations[index++] = i / 10.0f + offset;
                translations[index++] = j / 10.0f + offset;
            }
        }
        m_instanceVBO.create();
        m_instanceVBO.bind();
        m_instanceVBO.allocate(translations.data(), translations.size() * sizeof(GLfloat));
        m_instanceVBO.release();

        float quadVertices[] = {
            // positions     // colors
            -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
             0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
            -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

            -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
             0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
             0.05f,  0.05f,  0.0f, 1.0f, 1.0f
        };
        m_quadVAO.create();
        m_quadVAO.bind();
        m_quadVBO.create();
        m_quadVBO.bind();
        m_quadVBO.allocate(quadVertices, sizeof(quadVertices));
        m_shader->enableAttributeArray(0);
        m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 2, sizeof(GLfloat) * 5);
        m_shader->enableAttributeArray(1);
        m_shader->setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 2, 3, sizeof(GLfloat) * 5);
        // also set instance data
        m_instanceVBO.bind();
        m_shader->enableAttributeArray(2);
        m_shader->setAttributeBuffer(2, GL_FLOAT, 0, 2, 2 * sizeof(GLfloat)); // 另一个VBO的
        m_instanceVBO.release();
        glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
    }
    // 相机类初始化
    m_camera.init();
}

void InstancingQuads::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void InstancingQuads::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->bind();
    m_quadVAO.bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
    m_quadVAO.release();
    m_shader->release();
}

bool InstancingQuads::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void InstancingQuads::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    qDebug() << debugMessage.message();
}
