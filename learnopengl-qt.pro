QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    1.getting_started/1.1hello_window/HelloWindow.cpp \
    1.getting_started/2.1hello_triangle/HelloTriangle.cpp \
    1.getting_started/2.2hello_triangle_indexed/HelloTriangleIndexed.cpp \
    1.getting_started/2.3hello_triangle_exercise1/HelloTriangleExercise1.cpp \
    1.getting_started/2.4hello_triangle_exercise2/HelloTriangleExercise2.cpp \
    1.getting_started/2.5hello_triangle_exercise3/HelloTriangleExercise3.cpp \
    1.getting_started/3.2shaders_interpolation/ShadersInterpolation.cpp \
    1.getting_started/4.1textures/Textures.cpp \
    1.getting_started/4.4textures_exercise2/TexturesExercise2.cpp \
    1.getting_started/4.5textures_exercise3/TexturesExercise3.cpp \
    1.getting_started/4.6textures_exercise4/TexturesExercise4.cpp \
    1.getting_started/5.1transformations/Transformations.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    1.getting_started/1.1hello_window/HelloWindow.h \
    1.getting_started/2.1hello_triangle/HelloTriangle.h \
    1.getting_started/2.2hello_triangle_indexed/HelloTriangleIndexed.h \
    1.getting_started/2.3hello_triangle_exercise1/HelloTriangleExercise1.h \
    1.getting_started/2.4hello_triangle_exercise2/HelloTriangleExercise2.h \
    1.getting_started/2.5hello_triangle_exercise3/HelloTriangleExercise3.h \
    1.getting_started/3.2shaders_interpolation/ShadersInterpolation.h \
    1.getting_started/4.1textures/Textures.h \
    1.getting_started/4.4textures_exercise2/TexturesExercise2.h \
    1.getting_started/4.5textures_exercise3/TexturesExercise3.h \
    1.getting_started/4.6textures_exercise4/TexturesExercise4.h \
    1.getting_started/5.1transformations/Transformations.h \
    MainWindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc
