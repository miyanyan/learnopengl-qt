#ifndef MULTIPLELIGHTS_H
#define MULTIPLELIGHTS_H

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
#include <QRadioButton>
#include "../../utils/CameraUtil.h"

class MultipleLights : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Q_INVOKABLE MultipleLights(QWidget *parent = nullptr);
    ~MultipleLights();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual bool event(QEvent *e) override;

private:
    void handleRadio();

private:
    QOpenGLVertexArrayObject m_lightVAO;
    QOpenGLVertexArrayObject m_lightCubeVAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_lightShader;
    QOpenGLShaderProgram m_lightCubeShader;

    std::unique_ptr<QOpenGLTexture> m_textureDiffuse;
    std::unique_ptr<QOpenGLTexture> m_textureSpecular;

    QVector3D m_lightPos;

    QMatrix4x4 m_projection;

    std::vector<QVector3D> m_pointLightPositions;
    std::vector<QVector3D> m_cubePositions;
    std::vector<QMatrix4x4> m_models;

    CameraUtil m_camera;

    QRadioButton* m_radioButtons[5];

    //styles
    struct DirLight {
        QVector3D direction;

        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
    };

    struct PointLight {
        QVector3D position;

        float constant;
        float linear;
        float quadratic;

        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
    };

    struct SpotLight {
        QVector3D position;
        QVector3D direction;
        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;

        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
    };

    QVector4D m_backgroundColor;
    DirLight m_dirLight;
    SpotLight m_spotLight;
    std::vector<QVector3D> m_pointLightColors;
    std::vector<PointLight> m_pointLightValues;
};

#endif // MULTIPLELIGHTS_H
