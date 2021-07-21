# learnopengl-qt
learn opengl with Qt, and it's directory structure is mostly like the [origin one](https://github.com/JoeyDeVries/LearnOpenGL/tree/master/src).

## How to use
* download [Qt](https://www.qt.io/download)
* open the file ```learnopengl-qt.pro``` with QtCreator
* run it in debug/release mode
* select a widget to show
    ![](./image/window.png)

## How to add a new widget
* create a new class like this
    ```c++
    class MultipleLights : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
    {
        Q_OBJECT
    public:
        //Q_INVOKABLE is needed
        Q_INVOKABLE MultipleLights(QWidget *parent = nullptr);
        ~MultipleLights();

    protected:
        virtual void initializeGL() override;
        virtual void resizeGL(int w, int h) override;
        virtual void paintGL() override;
        
    private:
    };
    ```
* inclue it in ```MainWindow.h```
    ```c++
    #include "2.lighting/6.1multiple_lights/MultipleLights.h"
    ```
* add the class name and staticMetaObject to a hashMap in ```MainWindow.cpp```
    ```c++
    void MainWindow::registerMetaObject()
    {
        //add staticMetaObject of the new class you have created to a list
        m_metaObjectList << &MultipleLights::staticMetaObject;

        //then easily traverse the list and add QMetaObject to map and combox
        for(const QMetaObject* mo : m_metaObjectList){
            m_metaObjectMap.insert(mo->className(), mo);
            m_combox->addItem(mo->className());
        }
    }
    ```
