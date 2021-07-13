#ifndef CAMERAUTIL_H
#define CAMERAUTIL_H

#include <QWidget>
#include <QMatrix4x4>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QTime>
#include <QSet>
#include <QMap>

//https://blog.csdn.net/qq_40946921/article/details/106014771
class CameraUtil
{
public:
    enum CameraMovement{
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };
    CameraUtil(QWidget* widget);
    //初始化
    void init();
    //处理事件
    void handle(QEvent* event);
    //设置按键响应
    void setKeyMap(CameraMovement key, Qt::Key value);
    //移动速度
    float getMoveSpeed() const;
    void setMoveSpeed(float value);
    //鼠标灵敏度
    float getSensitivity() const;
    void setSensitivity(float value);
    //偏航角
    float getYaw() const;
    void setYaw(float value);
    //俯视角
    float getPitch() const;
    void setPitch(float value);
    //fov
    float getFov() const;
    void setFov(float value);
    //摄像机位置
    QVector3D getCameraPos() const;
    void setCameraPos(const QVector3D& value);
    //
    QVector3D getCameraUp() const;
    void setCameraUp(const QVector3D& value);
    //摄像机方向，由于方向是由偏航角和俯视角得到的，故不提供set函数
    QVector3D getCameraDirection() const;
    //MVP
    QMatrix4x4 getViewMatrix();

private:
    void handleMouseMoveEvent(QMouseEvent* event);
    void handleWheelEvent(QWheelEvent* event);
    void handleTimerEvent(QTimerEvent* event);
    void calculateCameraDirection();
    void calculateViewMatrix();

private:
    QWidget* m_widget;

    float m_sensitivity;
    float m_yaw;
    float m_pitch;
    float m_fov;

    QMatrix4x4 m_view;

    QVector3D m_cameraPos;
    QVector3D m_cameraDirection;
    QVector3D m_cameraUp;

    QMap<CameraMovement, Qt::Key> m_keyMap;
    QSet<int> m_pressedKeys;
    int m_timerId;
    float m_deltaTime;//当前帧与上一帧的时间差
    float m_lastFrame;//上一帧的时间
    float m_moveSpeed;

    QPoint m_lastMousePos;
};

#endif // CAMERAUTIL_H
