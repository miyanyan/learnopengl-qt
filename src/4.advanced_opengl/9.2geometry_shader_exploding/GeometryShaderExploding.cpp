#include "GeometryShaderExploding.h"

#include <QDebug>

GeometryShaderExploding::GeometryShaderExploding(QWidget *parent)
    : QOpenGLWidget(parent),
      m_camera(this)
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&](){update();});
    timer->start(100);
}

GeometryShaderExploding::~GeometryShaderExploding()
{
    makeCurrent();
    m_model->destory();
    doneCurrent();
}

void GeometryShaderExploding::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize(); // initializes in the current context, i.e. ctx
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &GeometryShaderExploding::handleLoggedMessage);
    logger->startLogging();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //shader初始化
    {
        m_shader = std::make_unique<ShaderUtil>(":/4.advanced_opengl/9.2geometry_shader_exploding/geometry_shader_exploding.vert",
                                                ":/4.advanced_opengl/9.2geometry_shader_exploding/geometry_shader_exploding.frag",
                                                ":/4.advanced_opengl/9.2geometry_shader_exploding/geometry_shader_exploding.geom");
    }
    // load model
    m_model = std::make_unique<ModelUtil>(QString("%1%2").arg(STR_SRC_PATH).arg("/resources/objects/nanosuit/nanosuit.obj"), context());
    // 相机类初始化
    m_camera.init();
}

void GeometryShaderExploding::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GeometryShaderExploding::paintGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->bind();
    // mvp
    auto projection = QMatrix4x4();
    projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    auto view = m_camera.getViewMatrix();
    auto model = QMatrix4x4();
    model.scale(0.1);
    m_shader->setUniformValue("projection", projection);
    m_shader->setUniformValue("view", view);
    m_shader->setUniformValue("model", model);
    // time
    m_shader->setUniformValue("time", GLfloat(QTime::currentTime().msecsSinceStartOfDay() / 1000.0));
    // draw
    m_model->Draw(m_shader.get());
    m_shader->release();
}

bool GeometryShaderExploding::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void GeometryShaderExploding::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    qDebug() << debugMessage.message();
}
