#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

struct Vertex
{
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
    QVector3D Tangent;
    QVector3D Bitangent;
};

struct Texture
{
    QOpenGLTexture texture;
    QString type;
    QString path;
    Texture():texture(QOpenGLTexture::Target2D){
        texture.create();
        texture.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
        texture.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
        texture.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
    }
};

class Mesh {
public:
    /*  网格数据  */
    std::vector<Vertex> vertices;               //顶点数据
    std::vector<unsigned int> indices;          //索引数组
    std::vector<Texture*> textures;             //纹理数据

    /*  函数  */
    Mesh(QOpenGLFunctions* glFunc);
    ~Mesh();

    void Draw(QOpenGLShaderProgram& shader);
    void Draw(QOpenGLShaderProgram* shader);
    void setupMesh();

private:
    /*  渲染数据  */
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO,m_EBO;
    //opengl函数入口
    QOpenGLFunctions* m_glFunc;
};

#endif // MESH_H
