#pragma once
#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <Magick++.h>

#ifdef USE_IMAGE_MAGICK
#include <Magick++.h>
#endif

class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);

    bool Load();
    void Load(unsigned int BufferSize, void* pData);

    void LoadRaw(int Width, int Height, int BPP, unsigned char* pData);


    void Bind(GLenum TextureUnit);

private:
    void LoadInternal(void* image_data);

    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;

    int m_imageWidth = 0;
    int m_imageHeight = 0;
    int m_imageBPP = 0;

    Magick::Image* m_pImage;
    Magick::Blob m_blob;
};


#endif	/* TEXTURE_H */