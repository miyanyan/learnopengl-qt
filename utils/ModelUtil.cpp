#include "ModelUtil.h"


ModelUtil::ModelUtil(QString path, QOpenGLContext* context)
    : m_context(context)
    , directory(path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(directory.absolutePath().toLocal8Bit(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << "ERROR::ASSIMP::"<<import.GetErrorString() << endl;
        return;
    }
    qDebug() << "ASSIMP has loaded :" << directory.absolutePath();
    directory.cdUp();
    processNode(scene->mRootNode, scene);
}

ModelUtil::~ModelUtil()
{
    // delete textures
    for(auto&it : m_texturesLoaded){
        delete it.second;
    }
}

void ModelUtil::Draw(QOpenGLShaderProgram &shader)
{
    for(auto& mesh : m_meshes){
        mesh->Draw(shader);
    }
}

void ModelUtil::destory()
{
    for(auto&it : m_texturesLoaded){
        it.second->texture.destroy();
    }
}

void ModelUtil::processNode(aiNode *node, const aiScene *scene)
{

    // 处理节点所有的网格（如果有的话）
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));

    }
    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

std::unique_ptr<Mesh> ModelUtil::processMesh(aiMesh *mesh, const aiScene *scene)
{
    //  初始化网格
    std::unique_ptr<Mesh> newMesh(new Mesh(m_context->functions()));
    // 遍历网格的每个顶点
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        // 创建一个顶点
        Vertex vertex;
        // 位置
        vertex.Position = QVector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        // 法向量N
        if (mesh->mNormals) {
            vertex.Normal = QVector3D(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        // 纹理坐标
        if (mesh->mTextureCoords[0]) {
            //一个顶点最多可以包含8个不同的纹理坐标。因此我们假设我们不用
            //使用一个顶点可以有多个纹理坐标的模型，所以我们总是取第一个集合(0)。
            vertex.TexCoords = QVector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.TexCoords = QVector2D(0,0);
        }
        // 切线T
        if (mesh->mTangents) {
            vertex.Tangent = QVector3D(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        }
        // 副切线B
        if (mesh->mBitangents) {
            vertex.Bitangent = QVector3D(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }
        // bitangent
        newMesh->vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        // 将所有面的索引数据添加到索引数组中
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            newMesh->indices.push_back(face.mIndices[j]);
        }
    }

    // 处理材质
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. 漫反射贴图
    std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    newMesh->textures.insert(newMesh->textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. 镜面贴图
    std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    newMesh->textures.insert(newMesh->textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. 法向量图
    std::vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    newMesh->textures.insert(newMesh->textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. 高度图
    std::vector<Texture*> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    newMesh->textures.insert(newMesh->textures.end(), heightMaps.begin(), heightMaps.end());

    // 在顶点索引加载完成后需要进行 setupMesh
    newMesh->setupMesh();
    return newMesh;
}

std::vector<Texture*> ModelUtil::loadMaterialTextures(aiMaterial *mat, aiTextureType type, QString typeName)
{
    std::vector<Texture*> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string strPath(str.C_Str());
        // 检查纹理是否在之前加载过，如果是，则继续到下一个迭代:跳过加载新纹理
        if (m_texturesLoaded.find(strPath) != m_texturesLoaded.end()) {
            textures.push_back(m_texturesLoaded[strPath]);
        } else {
            // 如果材质还没有加载，加载它
            QImage data(directory.filePath(str.C_Str()));
            if(!data.isNull()){
                Texture* texture = new Texture;
                texture->texture.setData(data);
                texture->type = typeName;
                texture->path = str.C_Str();
                textures.push_back(texture);
                m_texturesLoaded[strPath] = texture;  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
            else{
                qDebug() << "invalid texture : " << directory.filePath(str.C_Str());
            }
        }
    }
    return textures;
}
