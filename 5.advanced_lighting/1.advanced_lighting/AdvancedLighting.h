#ifndef ADVANCEDLIGHTING_H
#define ADVANCEDLIGHTING_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include "../../utils/CameraUtil.h"
#include "../../utils/ModelUtil.h"
#include "../../utils/ShaderUtil.h"

class AdvancedLighting : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE AdvancedLighting(QWidget *parent = nullptr);
    ~AdvancedLighting();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual bool event(QEvent *e) override;

private:
    QOpenGLVertexArrayObject m_planeVAO;
    QOpenGLBuffer m_planeVBO;

    std::unique_ptr<ShaderUtil> m_shader;

    std::unique_ptr<QOpenGLTexture> m_texture;

    CameraUtil m_camera;

    QSlider* m_sliderAmbientStrength;
    QSlider* m_sliderSpecularStrength;
    QSlider* m_sliderSpecularShininess;

    QCheckBox* m_checkBoxBlinn;
};

#endif // ADVANCEDLIGHTING_H
