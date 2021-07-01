#include "HelloWindow.h"

#include <QDebug>

HelloWindow::HelloWindow(QWidget *parent) : QOpenGLWidget(parent)
{

}

void HelloWindow::initializeGL()
{
    if(!context()){
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();
    qInfo() << "real OpenGL version " << reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

void HelloWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void HelloWindow::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
