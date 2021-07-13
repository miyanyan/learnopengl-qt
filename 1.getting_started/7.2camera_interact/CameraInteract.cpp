#include "CameraInteract.h"

#include <QDebug>
#include <QTime>
#include <QMessageBox>

#define M_PI 3.1415926

CameraInteract::CameraInteract(QWidget *parent)
    : QOpenGLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_cameraPos(0.0, 0.0, 3.0),
      m_cameraUp(0.0, 1.0, 0.0),
      m_timerId(0),
      m_moveSpeed(1.0),
      m_yaw(-90.0),
      m_pitch(0.0),
      m_sensitivity(0.01),
      m_fov(45.0)
{
    m_cameraDirection = {cos(m_yaw) * cos(m_pitch), sin(m_pitch), sin(m_yaw) * cos(m_pitch)};
    m_cameraDirection.normalize();

    setMouseTracking(true);
}

CameraInteract::~CameraInteract()
{
    makeCurrent();

    m_VBO.destroy();
    m_VAO.destroy();
    m_texture[0]->destroy();
    m_texture[1]->destroy();

    doneCurrent();
}

void CameraInteract::initializeGL()
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
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/1.getting_started/6.2coordinate_systems_depth/CoordinateSystemsDepth.vert");
        if(!result){
            qDebug() << m_program.log();
        }
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/1.getting_started/6.2coordinate_systems_depth/CoordinateSystemsDepth.frag");
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
        //2D rect
        GLfloat vertices[] = {
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
        //1. 绑定顶点数组对象
        m_VAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
        //2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        //3. 设置顶点属性指针
        //3.1 位置
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
        m_program.enableAttributeArray(0);
        //3.2 纹理
        m_program.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 2, sizeof(GLfloat) * 5);
        m_program.enableAttributeArray(1);
    }
    //texture
    m_texture[0] = std::make_unique<QOpenGLTexture>(QImage(":/1.getting_started/4.1textures/awesomeface.png").mirrored());
    m_texture[1] = std::make_unique<QOpenGLTexture>(QImage(":/1.getting_started/4.1textures/container.jpg").mirrored());
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
        m_models.resize(m_cubePositions.size());
        for(int i = 0; i < m_cubePositions.size(); ++i){
            m_models[i].translate(m_cubePositions[i]);
        }
        //m_view.translate({0.0, 0.0, -3.0});
        m_projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    }
    //
    m_lastMousePos = this->geometry().center();
    QCursor::setPos(mapToGlobal(m_lastMousePos));//鼠标在中间
    this->setCursor(Qt::BlankCursor);//鼠标不可见
    setFocus();
}

void CameraInteract::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void CameraInteract::paintGL()
{    
    //帧时间
    float curTime = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
    m_deltaTime = curTime - m_lastFrame;
    m_lastFrame = curTime;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.bind();
    m_texture[0]->bind(0);
    m_program.setUniformValue("texture1", 0);
    m_texture[1]->bind(1);
    m_program.setUniformValue("texture2", 1);
    //MVP
    m_view.setToIdentity();
    m_view.lookAt(m_cameraPos, m_cameraPos + m_cameraDirection, m_cameraUp);
    m_projection.setToIdentity();
    m_projection.perspective(m_fov, 1.0 * width() / height(), 0.1, 100.0);
    m_program.setUniformValue("view", m_view);
    m_program.setUniformValue("projection", m_projection);
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    for(int i = 0; i < m_cubePositions.size(); ++i){
        //m_models[i].rotate(10.0 , {0.5, 1.0, 0.0});
        m_program.setUniformValue("model", m_models[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    }

    m_texture[0]->release();
    m_texture[1]->release();
    m_program.release();
}

void CameraInteract::keyPressEvent(QKeyEvent *event)
{
    m_pressedKeys.insert(event->key());
    //启动定时器
    if(!event->isAutoRepeat() && m_timerId == 0){
        m_timerId = startTimer(1);
    }
}

void CameraInteract::keyReleaseEvent(QKeyEvent *event)
{
    m_pressedKeys.remove(event->key());
    //当没有按键按下且定时器正在运行，才关闭定时器
    if(!event->isAutoRepeat() && m_timerId != 0 && m_pressedKeys.empty()){
        killTimer(m_timerId);
        m_timerId = 0;
    }
}

void CameraInteract::timerEvent(QTimerEvent *event)
{
    float speed = m_deltaTime * m_moveSpeed;
    if(m_pressedKeys.contains(Qt::Key_W)){
        m_cameraPos += speed * m_cameraDirection;
    }
    if(m_pressedKeys.contains(Qt::Key_S)){
        m_cameraPos -= speed * m_cameraDirection;
    }
    if(m_pressedKeys.contains(Qt::Key_A)){
        auto cameraRight = QVector3D::crossProduct(m_cameraDirection, m_cameraUp);
        cameraRight.normalize();
        m_cameraPos -= speed * cameraRight;
    }
    if(m_pressedKeys.contains(Qt::Key_D)){
        auto cameraRight = QVector3D::crossProduct(m_cameraDirection, m_cameraUp);
        cameraRight.normalize();
        m_cameraPos += speed * cameraRight;
    }
    qDebug() << m_deltaTime << ", " << m_moveSpeed;
    update();
}

void CameraInteract::mouseMoveEvent(QMouseEvent *event)
{
    float offsetX = event->x() - m_lastMousePos.x();
    float offsetY = m_lastMousePos.y() - event->y();//注意坐标方向
    offsetX *= m_sensitivity;
    offsetY *= m_sensitivity;
    //更新lastpos
    m_lastMousePos = event->pos();
    //更新欧拉角
    m_yaw += offsetX;
    m_pitch += offsetY;
    if(m_pitch >= M_PI / 2.0){
        m_pitch = M_PI / 2.0 - 0.1;
    }
    else if(m_pitch <= -M_PI / 2.0){
        m_pitch = -M_PI / 2.0 + 0.1;
    }

    m_cameraDirection = {cos(m_yaw) * cos(m_pitch), sin(m_pitch), sin(m_yaw) * cos(m_pitch)};
    m_cameraDirection.normalize();

    update();
}

void CameraInteract::wheelEvent(QWheelEvent *event)
{
    auto degree = event->angleDelta();
    double factor = degree.y() / 360.0;

    m_fov += factor;
    if(m_fov >= 45.0){
        m_fov = 45.0;
    }
    else if(m_fov <= 1.0){
        m_fov = 1.0;
    }

    update();
}

