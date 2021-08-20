#include "OpenGLUniformbufferObject.h"

OpenGLUniformbufferObject::OpenGLUniformbufferObject(std::vector<GLuint> shaderIds, QString structName, QStringList elementNames)
    : m_shaderIds(shaderIds)
    , m_uboStructName(structName)
    , m_uboElementNames(elementNames)
    , m_ubo(0)
    , m_uboSize(0)
{
    // get current opengl function
    m_glFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    // get the relevant block indices, and link each shader's uniform block to this uniform binding point
    std::vector<GLuint> uniformBlockIndex(m_shaderIds.size());
    for (int i = 0; i < m_shaderIds.size(); ++i) {
        uniformBlockIndex[i] = m_glFunc->glGetUniformBlockIndex(m_shaderIds[i], m_uboStructName.toStdString().c_str());
        m_glFunc->glUniformBlockBinding(m_shaderIds[i], uniformBlockIndex[i], 0);
    }
    // resize
    int n = m_uboElementNames.size();
    m_uboIndices.resize(n);
    m_uboOffsets.resize(n);
    // get information
    m_charNames = new char*[n];
    for (int i = 0; i < n; ++i) {
        std::string name = m_uboElementNames[i].toStdString();
        m_charNames[i] = new char[name.size() + 1]; // to account for \0
        strcpy(m_charNames[i], name.c_str());
    }

    m_glFunc->glGetActiveUniformBlockiv(m_shaderIds[0], uniformBlockIndex[0], GL_UNIFORM_BLOCK_DATA_SIZE, &m_uboSize);
    m_glFunc->glGetUniformIndices(m_shaderIds[0], n, m_charNames, m_uboIndices.data());
    m_glFunc->glGetActiveUniformsiv(m_shaderIds[0], n, m_uboIndices.data(), GL_UNIFORM_OFFSET, m_uboOffsets.data());
}

OpenGLUniformbufferObject::~OpenGLUniformbufferObject()
{
    m_glFunc->glDeleteBuffers(1, &m_ubo);
    for (int i = 0; i < m_uboElementNames.size(); ++i) {
        delete[] m_charNames[i];
    }
    delete[] m_charNames;
}

void OpenGLUniformbufferObject::create(GLenum usage)
{
    if (m_ubo != 0) return;
    m_glFunc->glGenBuffers(1, &m_ubo);
    m_glFunc->glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    m_glFunc->glBufferData(GL_UNIFORM_BUFFER, m_uboSize, NULL, usage);
    m_glFunc->glBindBuffer(GL_UNIFORM_BUFFER, 0);
    m_glFunc->glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo, 0, m_uboSize);
}

void OpenGLUniformbufferObject::bind()
{
    m_glFunc->glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
}

void OpenGLUniformbufferObject::release()
{
    m_glFunc->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLUniformbufferObject::setData(int index, const GLvoid *data)
{
    int n = m_uboElementNames.size();
    if (index >= n) return;

    GLintptr offset = m_uboOffsets[index];
    GLsizeiptr size;
    if (index < n - 1) {
        size = m_uboOffsets[index + 1];
    } else {
        size = m_uboSize - offset;
    }
    m_glFunc->glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
