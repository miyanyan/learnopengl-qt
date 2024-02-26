#ifndef INSTANCINGQUADS_H
#define INSTANCINGQUADS_H

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

class InstancingQuads : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE InstancingQuads(QWidget *parent = nullptr);
    ~InstancingQuads();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

private:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

private:
    QOpenGLVertexArrayObject m_quadVAO;
    QOpenGLBuffer m_quadVBO, m_instanceVBO;
    std::unique_ptr<ShaderUtil> m_shader;

    CameraUtil m_camera;
};

#endif // INSTANCINGQUADS_H
