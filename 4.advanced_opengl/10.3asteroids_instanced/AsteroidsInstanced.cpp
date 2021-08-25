#include "AsteroidsInstanced.h"

#include <QDebug>

AsteroidsInstanced::AsteroidsInstanced(QWidget *parent)
    : QOpenGLWidget(parent),
      m_camera(this)
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
}

AsteroidsInstanced::~AsteroidsInstanced()
{
    makeCurrent();
    m_modelPlanet->destory();
    m_modelRock->destory();
    doneCurrent();
}

void AsteroidsInstanced::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize(); // initializes in the current context, i.e. ctx
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &AsteroidsInstanced::handleLoggedMessage);
    logger->startLogging();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //shader初始化
    {
        m_shader = std::make_unique<ShaderUtil>(":/4.advanced_opengl/10.3asteroids_instanced/planet.vert",
                                                ":/4.advanced_opengl/10.3asteroids_instanced/planet.frag");
        m_rockShader = std::make_unique<ShaderUtil>(":/4.advanced_opengl/10.3asteroids_instanced/asteroids.vert",
                                                    ":/4.advanced_opengl/10.3asteroids_instanced/asteroids.frag");
    }
    // load model
    {
        m_modelPlanet = std::make_unique<ModelUtil>(QString("%1%2").arg(STR_SRC_PATH).arg("/resources/objects/planet/planet.obj"), context());
        m_modelRock = std::make_unique<ModelUtil>(QString("%1%2").arg(STR_SRC_PATH).arg("/resources/objects/rock/rock.obj"), context());
    }
    // model matrices
    {
        int n = 100000;
        m_modelMatrices.resize(n);

        float radius = 150.0f;
        float offset = 25.0f;
        for (int i = 0; i < n; ++i) {
            QMatrix4x4 model;
            // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
            float angle = i * 360.0f / n;
            float displacement = (qrand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (qrand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
            displacement = (qrand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model.translate(x, y, z);
            // 2.缩放
            float scale = (qrand() % 20) / 100.0f + 0.05;
            model.scale(scale);
            // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
            float rotAngle = (qrand() % 360);
            model.rotate(rotAngle, QVector3D(0.4f, 0.6f, 0.8f));
            // 4. 添加到数组中
            memcpy(&m_modelMatrices[i], model.data(), sizeof(mat4));
        }
        m_VBO.create();
        m_VBO.bind();
        // 注意如果使用 QMatrix4x4, 大小和间隔都要重新计算, 并且 sizeof(QMatrix4x4) = 68, 多了个sizeof(flagBits), 计算起来麻烦
        m_VBO.allocate(m_modelMatrices.data(), n * sizeof(mat4));

        int vec4Size = 4 * sizeof(float);
        for (auto& mesh : m_modelRock->meshes) {
            auto VAO = mesh->getVAO();
            VAO->bind();
            // 顶点属性, 顶点属性最大允许的数据大小等于一个vec4， 所以一个mat4要用4个vec4
            m_rockShader->enableAttributeArray(3);
            m_rockShader->setAttributeBuffer(3, GL_FLOAT, 0 * vec4Size, 4, 4 * vec4Size);
            m_rockShader->enableAttributeArray(4);
            m_rockShader->setAttributeBuffer(4, GL_FLOAT, 1 * vec4Size, 4, 4 * vec4Size);
            m_rockShader->enableAttributeArray(5);
            m_rockShader->setAttributeBuffer(5, GL_FLOAT, 2 * vec4Size, 4, 4 * vec4Size);
            m_rockShader->enableAttributeArray(6);
            m_rockShader->setAttributeBuffer(6, GL_FLOAT, 3 * vec4Size, 4, 4 * vec4Size);
            // 渲染一个新实例的时候更新顶点属性
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            VAO->release();
        }
    }
    // 相机类初始化
    m_camera.init();
    m_camera.setCameraPos({0.0f, 0.0f, 155.0f});
}

void AsteroidsInstanced::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void AsteroidsInstanced::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = QMatrix4x4();
    projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1f, 1000.0f);
    auto view = m_camera.getViewMatrix();

    m_shader->bind();
    m_shader->setUniformValue("projection", projection);
    m_shader->setUniformValue("view", view);
    // draw planet
    auto model = QMatrix4x4();//m_modelPlanet->getNDCMatrix();
    model.translate(0.0f, -3.0f, 0.0f);
    model.scale(4.0f);
    m_shader->setUniformValue("model", model);
    m_modelPlanet->Draw(m_shader.get());

    // draw meteorites
    m_rockShader->bind();
    m_rockShader->setUniformValue("projection", projection);
    m_rockShader->setUniformValue("view", view);
    m_rockShader->setUniformValue("texture_diffuse1", 0);
    m_modelRock->texturesLoaded[0]->texture.bind();
    for (auto& mesh : m_modelRock->meshes) {
        auto VAO = mesh->getVAO();
        VAO->bind();
        glDrawElementsInstanced(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0, m_modelMatrices.size());
        VAO->release();
    }
    m_rockShader->release();
}

bool AsteroidsInstanced::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void AsteroidsInstanced::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    qDebug() << debugMessage.message();
}
