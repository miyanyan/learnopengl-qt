#ifndef CUBEMAPTEXTUREUTIL_H
#define CUBEMAPTEXTUREUTIL_H

#include <QOpenGLTexture>

// I tried RGB format and got a black screen here, which seems that cubemap wasn't allocated successfully
class CubemapTextureUtil : public QOpenGLTexture
{
public:
    CubemapTextureUtil(QString pathPositiveX,
                       QString pathPositiveY,
                       QString pathPositiveZ,
                       QString pathNegativeX,
                       QString pathNegativeY,
                       QString pathNegativeZ)
        : QOpenGLTexture(QOpenGLTexture::TargetCubeMap)
    {
        // load image
        auto posx = QImage(pathPositiveX).convertToFormat(QImage::Format_RGBA8888);
        auto posy = QImage(pathPositiveY).convertToFormat(QImage::Format_RGBA8888);
        auto posz = QImage(pathPositiveZ).convertToFormat(QImage::Format_RGBA8888);
        auto negx = QImage(pathNegativeX).convertToFormat(QImage::Format_RGBA8888);
        auto negy = QImage(pathNegativeY).convertToFormat(QImage::Format_RGBA8888);
        auto negz = QImage(pathNegativeZ).convertToFormat(QImage::Format_RGBA8888);
        // create and allocateStorage
        this->create();
        this->setSize(posx.width(), posx.height(), posx.depth());
        this->setFormat(QOpenGLTexture::RGBA8_UNorm);
        this->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
        this->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, posx.constBits());
        this->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, posy.constBits());
        this->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, posz.constBits());
        this->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negx.constBits());
        this->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negy.constBits());
        this->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negz.constBits());
    }
};

#endif // CUBEMAPTEXTUREUTIL_H
