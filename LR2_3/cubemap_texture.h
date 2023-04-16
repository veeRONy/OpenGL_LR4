#pragma once
#ifndef CUBEMAP_H
#define	CUBEMAP_H

#include <string>
#include <GL/glew.h>

// In the youtube series I've switch to stb_image
//#define USE_IMAGE_MAGICK

#ifdef USE_IMAGE_MAGICK
#include <Magick++.h>
#endif

using namespace std;

class CubemapTexture
{
public:

    CubemapTexture(const string& Directory,
        const string& PosXFilename,
        const string& NegXFilename,
        const string& PosYFilename,
        const string& NegYFilename,
        const string& PosZFilename,
        const string& NegZFilename);

    ~CubemapTexture();

    bool Load();

    void Bind(GLenum TextureUnit);

private:

    string m_fileNames[6];
    GLuint m_textureObj;
};

#endif	/* CUBEMAP_H */