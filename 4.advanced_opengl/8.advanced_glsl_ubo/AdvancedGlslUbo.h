#ifndef ADVANCEDGLSLUBO_H
#define ADVANCEDGLSLUBO_H

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
#include "../../utils/CameraUtil.h"
#include "../../utils/ShaderUtil.h"
#include "../../utils/OpenGLUniformbufferObject.h"

class AdvancedGlslUbo : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE AdvancedGlslUbo(QWidget *parent = nullptr);
    ~AdvancedGlslUbo();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

private:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

private:
    QOpenGLVertexArrayObject m_cubeVAO;
    QOpenGLBuffer m_cubeVBO;

    std::unique_ptr<OpenGLUniformbufferObject> m_UBO;

    std::unique_ptr<ShaderUtil> m_shaderRed;
    std::unique_ptr<ShaderUtil> m_shaderGreen;
    std::unique_ptr<ShaderUtil> m_shaderBlue;
    std::unique_ptr<ShaderUtil> m_shaderYellow;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;
};

#endif // ADVANCEDGLSLUBO_H
