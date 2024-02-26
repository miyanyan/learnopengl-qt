#include "AdvancedLighting.h"

#include <QDebug>

AdvancedLighting::AdvancedLighting(QWidget *parent)
    : QOpenGLWidget(parent),
      m_camera(this)
{
    m_checkBoxBlinn = new QCheckBox("blinn", this);
    m_checkBoxBlinn->setStyleSheet("QCheckBox { color : white; }");
    connect(m_checkBoxBlinn, &QCheckBox::clicked, this, [&](){update();});

    QStringList text;
    text << "ambient strength" << "specular strength" << "specular shininess";
    for(int i = 0; i < text.size(); ++i){
        QLabel* label = new QLabel(text[i], this);
        label->setStyleSheet("QLabel { color : white; }");
        label->setGeometry(10, 20 * i + 20, 120, 20);
    }
    m_sliderAmbientStrength = new QSlider(Qt::Horizontal, this);
    m_sliderSpecularStrength = new QSlider(Qt::Horizontal, this);
    m_sliderSpecularShininess = new QSlider(Qt::Horizontal, this);

    m_sliderAmbientStrength->setGeometry(140, 0, 80, 20);
    m_sliderSpecularStrength->setGeometry(140, 20, 80, 20);
    m_sliderSpecularShininess->setGeometry(140, 40, 80, 20);

    m_sliderAmbientStrength->setMaximum(1000);
    m_sliderSpecularStrength->setMaximum(1000);
    m_sliderSpecularShininess->setMaximum(1000);

    m_sliderAmbientStrength->setMinimum(1);
    m_sliderSpecularStrength->setMinimum(1);
    m_sliderSpecularShininess->setMinimum(1);

    connect(m_sliderAmbientStrength, &QSlider::valueChanged, this, [&](){update();});
    connect(m_sliderSpecularStrength, &QSlider::valueChanged, this, [&](){update();});
    connect(m_sliderSpecularShininess, &QSlider::valueChanged, this, [&](){update();});
}

AdvancedLighting::~AdvancedLighting()
{
    makeCurrent();

    m_texture->destroy();

    doneCurrent();
}

void AdvancedLighting::initializeGL()
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
        m_shader = std::make_unique<ShaderUtil>(":/5.advanced_lighting/1.advanced_lighting/advanced_lighting.vert",
                                                ":/5.advanced_lighting/1.advanced_lighting/advanced_lighting.frag");
    }
    //
    {
        float planeVertices[] = {
            // positions            // normals         // texcoords
             10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
            -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

             10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
             10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
        };
        m_planeVAO.create();
        m_planeVAO.bind();
        m_planeVBO.create();
        m_planeVBO.bind();
        m_planeVBO.allocate(planeVertices, sizeof(planeVertices));
        m_shader->enableAttributeArray(0);
        m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
        m_shader->enableAttributeArray(1);
        m_shader->setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 8);
        m_shader->enableAttributeArray(2);
        m_shader->setAttributeBuffer(2, GL_FLOAT, sizeof(GLfloat) * 6, 2, sizeof(GLfloat) * 8);
    }
    // texture
    {
        m_texture = std::make_unique<QOpenGLTexture>(QImage(":/resources/textures/wood.png").mirrored());
        m_shader->bind();
        m_shader->setUniformValue("texture1", 0);
    }
    //相机类初始化
    m_camera.init();
}

void AdvancedLighting::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void AdvancedLighting::paintGL()
{    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->bind();
    QMatrix4x4 projection;
    projection.perspective(m_camera.getFov(), width() * 1.0 / height(), 0.1f, 100.0f);
    QMatrix4x4 view = m_camera.getViewMatrix();
    m_shader->setUniformValue("projection", projection);
    m_shader->setUniformValue("view", view);
    // light
    m_shader->setUniformValue("viewPos", m_camera.getCameraPos());
    m_shader->setUniformValue("lightPos", QVector3D(0.0, 0.0, 0.0));
    m_shader->setUniformValue("blinn", (GLboolean)(m_checkBoxBlinn->isChecked()));
    m_shader->setUniformValue("ambientStrength", (GLfloat)(m_sliderAmbientStrength->value() / 1000.0));
    m_shader->setUniformValue("specularStrength", (GLfloat)(m_sliderSpecularStrength->value() / 1000.0));
    m_shader->setUniformValue("specularShininess", (GLfloat)(m_sliderSpecularShininess->value() / 1.0));
    // floor
    m_planeVAO.bind();
    m_texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_texture->release();
    m_planeVAO.release();
    m_shader->release();
}

bool AdvancedLighting::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}
