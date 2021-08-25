#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLDebugLogger>
#include <QMatrix4x4>
#include <QTimer>
#include <memory>
#include "Config.h"
#include "../../utils/ModelUtil.h"
#include "../../utils/CameraUtil.h"
#include "../../utils/ShaderUtil.h"
#include "../../utils/OpenGLUniformbufferObject.h"

class Asteroids : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE Asteroids(QWidget *parent = nullptr);
    ~Asteroids();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

private:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

private:
    std::unique_ptr<ShaderUtil> m_shader;

    std::unique_ptr<ModelUtil> m_modelPlanet;
    std::unique_ptr<ModelUtil> m_modelRock;

    std::vector<QMatrix4x4> m_modelMatrices;

    CameraUtil m_camera;
};

#endif // ASTEROIDS_H
