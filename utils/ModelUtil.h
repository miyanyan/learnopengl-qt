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

private:
    // opengl函数入口
    QOpenGLContext* m_context;
    /*  模型数据  */
    std::unordered_map<std::string, Texture*> m_texturesLoaded; // 缓存已加载的纹理
    std::vector<std::unique_ptr<Mesh>> m_meshes;                //网格
    QDir directory;                                             //模型所在路径

    //递归遍历结点
    void processNode(aiNode *node, const aiScene *scene);
    //加载网格
    std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    //加载材质纹理
    std::vector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type,QString typeName);
};
#endif // MODEL_H
