#ifndef HELLOWINDOW_H
#define HELLOWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMetaMethod>

class HelloWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Q_INVOKABLE HelloWindow(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // HELLOWINDOW_H
