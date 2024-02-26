#include "GeometryShaderNormals.h"

#include <QDebug>

GeometryShaderNormals::GeometryShaderNormals(QWidget *parent)
    : QOpenGLWidget(parent),
      m_camera(this)
{
}

GeometryShaderNormals::~GeometryShaderNormals()
{
    makeCurrent();
    m_model->destory();
    doneCurrent();
}

void GeometryShaderNormals::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize(); // initializes in the current context, i.e. ctx
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &GeometryShaderNormals::handleLoggedMessage);
    logger->startLogging();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //shader初始化
    {
        m_normalShader = std::make_unique<ShaderUtil>(":/4.advanced_opengl/9.3geometry_shader_normals/normal_visualization.vert",
                                                      ":/4.advanced_opengl/9.3geometry_shader_normals/normal_visualization.frag",
                                                      ":/4.advanced_opengl/9.3geometry_shader_normals/normal_visualization.geom");
        m_shader = std::make_unique<ShaderUtil>(":/4.advanced_opengl/9.3geometry_shader_normals/default.vert",
                                                ":/4.advanced_opengl/9.3geometry_shader_normals/default.frag");
    }
    // load model
    m_model = std::make_unique<ModelUtil>(QString("%1%2").arg(STR_SRC_PATH).arg("/resources/objects/nanosuit/nanosuit.obj"), context());
    // 相机类初始化
    m_camera.init();
}

void GeometryShaderNormals::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GeometryShaderNormals::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->bind();
    // mvp
    auto projection = QMatrix4x4();
    projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    auto view = m_camera.getViewMatrix();
    auto model = QMatrix4x4();
    //model.scale(0.1);
    m_shader->setUniformValue("projection", projection);
    m_shader->setUniformValue("view", view);
    m_shader->setUniformValue("model", model);
    // draw model as usual
    m_model->Draw(m_shader.get());

    // then draw model with normal visualizing geometry shader
    m_normalShader->bind();
    m_normalShader->setUniformValue("projection", projection);
    m_normalShader->setUniformValue("view", view);
    m_normalShader->setUniformValue("model", model);
    m_model->Draw(m_normalShader.get());
    m_normalShader->release();
}

bool GeometryShaderNormals::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void GeometryShaderNormals::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    qDebug() << debugMessage.message();
}
