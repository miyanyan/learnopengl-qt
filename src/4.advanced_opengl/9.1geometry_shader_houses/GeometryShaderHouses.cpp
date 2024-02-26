#include "GeometryShaderHouses.h"

#include <QDebug>

GeometryShaderHouses::GeometryShaderHouses(QWidget *parent)
    : QOpenGLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_camera(this)
{
}

GeometryShaderHouses::~GeometryShaderHouses()
{
    makeCurrent();

    doneCurrent();
}

void GeometryShaderHouses::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize(); // initializes in the current context, i.e. ctx
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &GeometryShaderHouses::handleLoggedMessage);
    logger->startLogging();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //shader初始化
    {
        m_shader = std::make_unique<ShaderUtil>(":/4.advanced_opengl/9.1geometry_shader_houses/geometry_shader_houses.vert",
                                                ":/4.advanced_opengl/9.1geometry_shader_houses/geometry_shader_houses.frag",
                                                ":/4.advanced_opengl/9.1geometry_shader_houses/geometry_shader_houses.geom");
    }
    //
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        GLfloat points[] = {
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
        };

        m_VAO.create();
        m_VAO.bind();
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(points, sizeof(points));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        m_VAO.release();
    }
    // 相机类初始化
    m_camera.init();
}

void GeometryShaderHouses::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GeometryShaderHouses::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw points
    m_shader->bind();
    m_VAO.bind();
    glDrawArrays(GL_POINTS, 0, 4);
    m_VAO.release();
    m_shader->release();
}

bool GeometryShaderHouses::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void GeometryShaderHouses::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    qDebug() << debugMessage.message();
}
