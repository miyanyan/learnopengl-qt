#ifndef CUBEMAPSENVIRONMENTMAPPING_H
#define CUBEMAPSENVIRONMENTMAPPING_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <memory>
#include "../../utils/ModelUtil.h"
#include "../../utils/CameraUtil.h"
#include "../../utils/ShaderUtil.h"
#include "../../utils/CubemapTextureUtil.h"

class CubemapsEnvironmentMapping : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE CubemapsEnvironmentMapping(QWidget *parent = nullptr);
    ~CubemapsEnvironmentMapping();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

private:
    QOpenGLVertexArrayObject m_cubeVAO, m_skyboxVAO;
    QOpenGLBuffer m_cubeVBO, m_skyboxVBO;

    std::unique_ptr<ModelUtil> m_assimpModel;

    std::unique_ptr<ShaderUtil> m_shader;
    std::unique_ptr<ShaderUtil> m_skyboxShader;

    std::unique_ptr<CubemapTextureUtil> m_cubemapTexture;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;

    bool m_isRefract;
};

#endif // CUBEMAPSENVIRONMENTMAPPING_H
