#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <unordered_map>
#include <QDir>
#include <QOpenGLTexture>
#include <QOpenGLContext>

#include "mesh.h"

class ModelUtil
{
public:
    ModelUtil(QString path, QOpenGLContext* context);
    ~ModelUtil();
    void Draw(QOpenGLShaderProgram& shader);
    void Draw(QOpenGLShaderProgram* shader);
    void destory();
    QMatrix4x4 getNDCMatrix();
    float getNDCScaleFactor();
    // 数据
    std::vector<std::unique_ptr<Mesh>> meshes; //网格
    std::vector<Texture*> texturesLoaded; // 缓存已加载的纹理

private:
    // opengl函数入口
    QOpenGLContext* m_context;
    QDir directory; //模型所在路径

    //递归遍历结点
    void processNode(aiNode *node, const aiScene *scene);
    //加载网格
    std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    //加载材质纹理
    std::vector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type,QString typeName);

    // to NDC
    void calculateNDCParam();
    float m_positiveX, m_positiveY, m_positiveZ;
    float m_negativeX, m_negativeY, m_negativeZ;
    float m_scaleFactor;
    QMatrix4x4 m_modelMatrix;
};
#endif // MODEL_H
