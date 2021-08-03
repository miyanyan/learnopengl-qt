
#include "mesh.h"
#include <QtOpenGLExtensions/QOpenGLExtensions>

Mesh::Mesh(QOpenGLFunctions* glFunc)
    : m_glFunc(glFunc)
    , m_VBO(QOpenGLBuffer::VertexBuffer)
    , m_EBO(QOpenGLBuffer::IndexBuffer)
{

}

Mesh::~Mesh()
{

}

void Mesh::Draw(QOpenGLShaderProgram &shader)
{
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for (unsigned int i = 0; i < textures.size(); ++i) {
        m_glFunc->glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        QString number;
        QString name = textures[i]->type;
        if (name == "texture_diffuse")
            number = QString::number(diffuseNr++);
        else if (name == "texture_specular")
            number = QString::number(specularNr++);
        else if (name == "texture_normal")
            number = QString::number(normalNr++);
        else if (name == "texture_height")
            number = QString::number(heightNr++);
        textures[i]->texture.bind();
        shader.setUniformValue((name + number).toStdString().c_str(),i);
    }
    // 画网格
    QOpenGLVertexArrayObject::Binder bind(&m_VAO);
    m_glFunc->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

/*!
 * \brief Mesh::setupMesh 绑定顶点和索引
 */
void Mesh::setupMesh()
{

    m_VAO.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);

    m_VBO.create();
    m_EBO.create();

    m_VBO.bind();
    m_VBO.allocate(vertices.data(),vertices.size() * sizeof(Vertex));

    m_EBO.bind();
    m_EBO.allocate(indices.data(),indices.size() * sizeof(unsigned int));


    // set the vertex attribute pointers
    // vertex Positions
    m_glFunc->glEnableVertexAttribArray(0);
    m_glFunc->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    m_glFunc->glEnableVertexAttribArray(1);
    m_glFunc->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    m_glFunc->glEnableVertexAttribArray(2);
    m_glFunc->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    m_glFunc->glEnableVertexAttribArray(3);
    m_glFunc->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    m_glFunc->glEnableVertexAttribArray(4);
    m_glFunc->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
}
