#ifndef ASTEROIDSINSTANCED_H
#define ASTEROIDSINSTANCED_H

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

class AsteroidsInstanced : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE AsteroidsInstanced(QWidget *parent = nullptr);
    ~AsteroidsInstanced();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

private:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

private:
    QOpenGLBuffer m_VBO;
    std::unique_ptr<ShaderUtil> m_shader;
    std::unique_ptr<ShaderUtil> m_rockShader;

    std::unique_ptr<ModelUtil> m_modelPlanet;
    std::unique_ptr<ModelUtil> m_modelRock;

    struct mat4
    {
        float m11;
        float m12;
        float m13;
        float m14;
        float m21;
        float m22;
        float m23;
        float m24;
        float m31;
        float m32;
        float m33;
        float m34;
        float m41;
        float m42;
        float m43;
        float m44;
    };
    std::vector<mat4> m_modelMatrices;

    CameraUtil m_camera;
};

#endif // ASTEROIDSINSTANCED_H
