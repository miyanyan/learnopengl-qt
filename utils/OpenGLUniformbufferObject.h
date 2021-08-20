#ifndef OPENGLUNIFORMBUFFEROBJECT_H
#define OPENGLUNIFORMBUFFEROBJECT_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>

class OpenGLUniformbufferObject
{
public:
    // shaderIds : numbers of shaders which has the same structName, must not be empty
    OpenGLUniformbufferObject(std::vector<GLuint> shaderIds, QString structName, QStringList elementNames);
    ~OpenGLUniformbufferObject();
    // 创建+绑定+解绑
    void create(GLenum usage = GL_STATIC_DRAW);
    void bind();
    void release();
    // 设置某一个数据
    void setData(int index, const GLvoid* data);

private:
    QOpenGLFunctions_3_3_Core* m_glFunc;
    // UBO
    std::vector<GLuint> m_shaderIds;
    QString m_uboStructName;
    QStringList m_uboElementNames;
    char** m_charNames;

    GLuint m_ubo;
    GLint m_uboSize;
    std::vector<GLint> m_uboOffsets;
    std::vector<GLuint> m_uboIndices;
};

#endif // OPENGLUNIFORMBUFFEROBJECT_H
