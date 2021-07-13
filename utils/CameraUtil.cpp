#include "CameraUtil.h"

#include <QDebug>

CameraUtil::CameraUtil(QWidget *widget)
    : m_widget(widget),
      m_yaw(-90.0),
      m_pitch(0.0),
      m_moveSpeed(1.0),
      m_sensitivity(0.01),
      m_fov(45.0),
      m_lastFrame(0.0),
      m_deltaTime(0.0),
      m_cameraPos(0.0, 0.0, 3.0),
      m_cameraUp(0.0, 1.0, 0.0)
{
    calculateCameraDirection();
    m_keyMap[FORWARD]   = Qt::Key_W;
    m_keyMap[BACKWARD]  = Qt::Key_S;
    m_keyMap[LEFT]      = Qt::Key_A;
    m_keyMap[RIGHT]     = Qt::Key_D;
}

void CameraUtil::init()
{
    m_widget->activateWindow();
    m_widget->setFocus();
}

void CameraUtil::handle(QEvent *event)
{
    switch (event->type()) {
        //鼠标移动
        case QEvent::MouseMove:
        {
            auto mouseEvent = static_cast<QMouseEvent*>(event);
            handleMouseMoveEvent(mouseEvent);
            break;
        }
        //定时器
        case QEvent::Timer:
        {
            auto timerEvent = static_cast<QTimerEvent*>(event);
            handleTimerEvent(timerEvent);
            break;
        }
        //滚轮
        case QEvent::Wheel:
        {
            auto wheelEvent = static_cast<QWheelEvent*>(event);
            handleWheelEvent(wheelEvent);
            break;
        }
        //按键按下
        case QEvent::KeyPress:
        {
            auto keyPressEvent = static_cast<QKeyEvent*>(event);
            m_pressedKeys.insert(keyPressEvent->key());
            //启动定时器
            if(!keyPressEvent->isAutoRepeat() && m_timerId == 0){
                m_timerId = m_widget->startTimer(1);
            }
            break;
        }
        //按键松开
        case QEvent::KeyRelease:
        {
            auto keyReleaseEvent = static_cast<QKeyEvent*>(event);
            m_pressedKeys.remove(keyReleaseEvent->key());
            //当没有按键按下且定时器正在运行，才关闭定时器
            if(!keyReleaseEvent->isAutoRepeat() && m_timerId != 0 && m_pressedKeys.empty()){
                m_widget->killTimer(m_timerId);
                m_timerId = 0;
            }
            break;
        }
        //刷新
        case QEvent::Paint:
        {
            //帧时间
            float curTime = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
            m_deltaTime = curTime - m_lastFrame;
            m_lastFrame = curTime;
            break;
        }
        //刷新
        case QEvent::FocusIn:
        {
            m_lastMousePos = m_widget->geometry().center();
            QCursor::setPos(m_widget->mapToGlobal(m_lastMousePos));//鼠标在中间
            m_widget->setCursor(Qt::BlankCursor);//鼠标不可见
            m_widget->setMouseTracking(true);
            break;
        }
    }
}

//************************************** get set ****************************************************

void CameraUtil::setKeyMap(CameraUtil::CameraMovement key, Qt::Key value)
{
    m_keyMap[key] = value;
}

float CameraUtil::getMoveSpeed() const
{
    return m_moveSpeed;
}

void CameraUtil::setMoveSpeed(float value)
{
    m_moveSpeed = value;
}

float CameraUtil::getSensitivity() const
{
    return m_sensitivity;
}

void CameraUtil::setSensitivity(float value)
{
    m_sensitivity = value;
}

float CameraUtil::getYaw() const
{
    return m_yaw;
}

void CameraUtil::setYaw(float value)
{
    m_yaw = value;
    calculateCameraDirection();
}

float CameraUtil::getPitch() const
{
    return m_pitch;
}

void CameraUtil::setPitch(float value)
{
    m_pitch = value;
    calculateCameraDirection();
}

float CameraUtil::getFov() const
{
    return m_fov;
}

void CameraUtil::setFov(float value)
{
    m_fov = value;
}

QVector3D CameraUtil::getCameraPos() const
{
    return m_cameraPos;
}

void CameraUtil::setCameraPos(const QVector3D &value)
{
    m_cameraPos = value;
}

QVector3D CameraUtil::getCameraUp() const
{
    return m_cameraUp;
}

void CameraUtil::setCameraUp(const QVector3D &value)
{
    m_cameraUp = value;
}

QVector3D CameraUtil::getCameraDirection() const
{
    return m_cameraDirection;
}

QMatrix4x4 CameraUtil::getViewMatrix()
{
    calculateViewMatrix();
    return m_view;
}


//***************************************** handle event *********************************************

void CameraUtil::handleMouseMoveEvent(QMouseEvent *event)
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
    //89° ≈ 1.55
    if(m_pitch > 1.55){
        m_pitch = 1.55;
    }
    else if(m_pitch < -1.55){
        m_pitch = -1.55;
    }

    calculateCameraDirection();

    m_widget->update();
}

void CameraUtil::handleWheelEvent(QWheelEvent *event)
{
    m_fov += event->angleDelta().y() / 360.0;
    if(m_fov > 45.0){
        m_fov = 45.0;
    }
    else if(m_fov < 1.0){
        m_fov = 1.0;
    }
    m_widget->update();
}

void CameraUtil::handleTimerEvent(QTimerEvent *event)
{
    //是否是同一个 timer
    if(event->timerId() != m_timerId){
        return;
    }
    float speed = m_deltaTime * m_moveSpeed;
    if(m_pressedKeys.contains(m_keyMap[FORWARD])){
        m_cameraPos += speed * m_cameraDirection;
    }
    if(m_pressedKeys.contains(m_keyMap[BACKWARD])){
        m_cameraPos -= speed * m_cameraDirection;
    }
    if(m_pressedKeys.contains(m_keyMap[LEFT])){
        auto cameraRight = QVector3D::crossProduct(m_cameraDirection, m_cameraUp);
        cameraRight.normalize();
        m_cameraPos -= speed * cameraRight;
    }
    if(m_pressedKeys.contains(m_keyMap[RIGHT])){
        auto cameraRight = QVector3D::crossProduct(m_cameraDirection, m_cameraUp);
        cameraRight.normalize();
        m_cameraPos += speed * cameraRight;
    }

    m_widget->update();
}


//**************************************** calculate method ********************************************
/*!
 * \brief CameraUtil::calculateCameraDirection 更新摄像机坐标系
 */
void CameraUtil::calculateCameraDirection()
{
    m_cameraDirection = QVector3D(cos(m_yaw) * cos(m_pitch), sin(m_pitch), sin(m_yaw) * cos(m_pitch));
    m_cameraDirection.normalize();
}

/*!
 * \brief CameraUtil::calculateViewMatrix 更新 view 矩阵
 */
void CameraUtil::calculateViewMatrix()
{
    m_view.setToIdentity();
    m_view.lookAt(m_cameraPos, m_cameraPos + m_cameraDirection, m_cameraUp);
}
