#include "MainWindow.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      m_openglWidget(nullptr),
      m_layout(nullptr)
{
    m_combox = new QComboBox(this);
    QStringList comboxList;
    comboxList << "HelloWindow" << "HelloTriangle" << "HelloTriangleIndexed"
               << "HelloTriangleExercise1" << "HelloTriangleExercise2" << "HelloTriangleExercise3"
               << "ShadersInterpolation"
               << "Textures" << "TexturesExercise2" << "TexturesExercise3" << "TexturesExercise4"
               << "Transformations";
    m_combox->addItems(comboxList);

    m_container = new QWidget(this);
    m_layout = new QGridLayout(m_container);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(m_combox, 1, 1, 1, 1);
    layout->addWidget(m_container, 2, 1, 10, 1);

    showGLWindows(0);

    connect(m_combox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::showGLWindows);

}

MainWindow::~MainWindow()
{
}

void MainWindow::showGLWindows(int index)
{
    if(m_openglWidget){
        m_layout->removeWidget(m_openglWidget);
        delete m_openglWidget;
    }

    switch (index) {
        case 0:
            m_openglWidget = new HelloWindow;
            break;
        case 1:
            m_openglWidget = new HelloTriangle;
            break;
        case 2:
            m_openglWidget = new HelloTriangleIndexed;
            break;
        case 3:
            m_openglWidget = new HelloTriangleExercise1;
            break;
        case 4:
            m_openglWidget = new HelloTriangleExercise2;
            break;
        case 5:
            m_openglWidget = new HelloTriangleExercise3;
            break;
        case 6:
            m_openglWidget = new ShadersInterpolation;
            break;
        case 7:
            m_openglWidget = new Textures;
            break;
        case 8:
            m_openglWidget = new TexturesExercise2;
            break;
        case 9:
            m_openglWidget = new TexturesExercise3;
            break;
        case 10:
            m_openglWidget = new TexturesExercise4;
            break;
        case 11:
            m_openglWidget = new Transformations;
            break;
        default:
            break;
    }
    m_layout->addWidget(m_openglWidget);
}

