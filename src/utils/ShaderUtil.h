#ifndef SHADERUTIL_H
#define SHADERUTIL_H

#include <QOpenGLShaderProgram>


class ShaderUtil : public QOpenGLShaderProgram
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
    ShaderUtil(QString vertexPath, QString fragmentPath, QString geometryPath) {
        bool result = true;
        result = addShaderFromSourceFile(QOpenGLShader::Vertex, vertexPath);
        if(!result){
            qDebug() << log();
        }
        result = addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentPath);
        if(!result){
            qDebug() << log();
        }
        result = addShaderFromSourceFile(QOpenGLShader::Geometry, geometryPath);
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
