#ifndef SHADERUTIL_H
#define SHADERUTIL_H

#include <QOpenGLShaderProgram>


class ShaderUtil : QOpenGLShaderProgram
{
public:
    ShaderUtil(QString vertexPath, QString fragmentPath) {
        bool result = true;
        result = addShaderFromSourceFile(QOpenGLShader::Vertex, vertexPath);
        if(!result){
            qDebug() << log();
        }
        result = addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentPath);
        if(!result){
            qDebug() << log();
        }
        result = link();
        if(!result){
            qDebug() << log();
        }
    }
};

#endif // SHADERUTIL_H