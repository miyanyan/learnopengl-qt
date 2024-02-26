#include "AdvancedGlslUbo.h"

#include <QDebug>

AdvancedGlslUbo::AdvancedGlslUbo(QWidget *parent)
    : QOpenGLWidget(parent),
      m_cubeVBO(QOpenGLBuffer::VertexBuffer),
      m_camera(this)
{
}

AdvancedGlslUbo::~AdvancedGlslUbo()
{
    makeCurrent();

    doneCurrent();
}

void AdvancedGlslUbo::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize(); // initializes in the current context, i.e. ctx
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &AdvancedGlslUbo::handleLoggedMessage);
    logger->startLogging();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //shader初始化
    {
        m_shaderBlue    = std::make_unique<ShaderUtil>(":/4.advanced_opengl/8.advanced_glsl_ubo/advanced_glsl.vert", ":/4.advanced_opengl/8.advanced_glsl_ubo/blue.frag");
        m_shaderRed     = std::make_unique<ShaderUtil>(":/4.advanced_opengl/8.advanced_glsl_ubo/advanced_glsl.vert", ":/4.advanced_opengl/8.advanced_glsl_ubo/red.frag");
        m_shaderGreen   = std::make_unique<ShaderUtil>(":/4.advanced_opengl/8.advanced_glsl_ubo/advanced_glsl.vert", ":/4.advanced_opengl/8.advanced_glsl_ubo/green.frag");
        m_shaderYellow  = std::make_unique<ShaderUtil>(":/4.advanced_opengl/8.advanced_glsl_ubo/advanced_glsl.vert", ":/4.advanced_opengl/8.advanced_glsl_ubo/yellow.frag");
    }
    //
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        GLfloat cubeVertices[] = {
            // positions
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        m_cubeVAO.create();
        m_cubeVAO.bind();
        m_cubeVBO.create();
        m_cubeVBO.bind();
        m_cubeVBO.allocate(cubeVertices, sizeof(cubeVertices));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        m_cubeVAO.release();
    }
    // UBO
    {
        // 1. 创建UBO
        std::vector<GLuint> shaderIds;
        shaderIds.emplace_back(m_shaderBlue->programId());
        shaderIds.emplace_back(m_shaderGreen->programId());
        shaderIds.emplace_back(m_shaderRed->programId());
        shaderIds.emplace_back(m_shaderYellow->programId());
        QStringList uboNames;
        uboNames << "projection" << "view";
        m_UBO = std::make_unique<OpenGLUniformbufferObject>(shaderIds, "Matrices", uboNames);
        m_UBO->create();
        // 2. 假定projection 不变， 只需设置一次
        m_projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
        m_UBO->bind();
        m_UBO->setData(0, m_projection.data());
        m_UBO->release();
    }
    // 相机类初始化
    m_camera.init();
}

void AdvancedGlslUbo::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void AdvancedGlslUbo::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // UBO, 只需要更新view
    auto view = m_camera.getViewMatrix();
    m_UBO->bind();
    m_UBO->setData(1, view.data());
    m_UBO->release();
    // 4 cubes
    m_cubeVAO.bind();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_cubeVAO);
    // blue
    m_shaderBlue->bind();
    m_model.setToIdentity();
    m_model.translate(-0.75f, 0.75f, 0.0f);
    m_shaderBlue->setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Green
    m_shaderGreen->bind();
    m_model.setToIdentity();
    m_model.translate(0.75f, 0.75f, 0.0f);
    m_shaderGreen->setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // red
    m_shaderRed->bind();
    m_model.setToIdentity();
    m_model.translate(-0.75f, -0.75f, 0.0f);
    m_shaderRed->setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // yellow
    m_shaderYellow->bind();
    m_model.setToIdentity();
    m_model.translate(0.75f, -0.75f, 0.0f);
    m_shaderYellow->setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_shaderYellow->release();
}

bool AdvancedGlslUbo::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void AdvancedGlslUbo::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    qDebug() << debugMessage.message();
}
