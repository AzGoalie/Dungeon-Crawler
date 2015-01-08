#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{
	m_Handle = 0;
	m_Wrap = GL_CLAMP_TO_EDGE;
	m_Filter = GL_NEAREST;

	m_Width = 0;
	m_Height = 0;
    m_Comp = 0;

	m_Loaded = false;
}

Texture::~Texture()
{
	if (m_Loaded)
        Destory();
}

void Texture::SetWrap(GLuint wrap)
{
    m_Wrap = wrap;
    if (m_Loaded)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Wrap);
    }
}

void Texture::SetFilter(GLuint filter)
{
    m_Filter = filter;
    if (m_Loaded)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Filter);
    }
}

bool Texture::Load(const char* filename)
{
	if (m_Loaded)
		Destory();
    
    glGenTextures(1, &m_Handle);
    FILE* image = fopen(filename, "rb");
    if (!image)
        return false;
    unsigned char* data = stbi_load_from_file(image, &m_Width, &m_Height, &m_Comp, 0);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    fclose(image);
    stbi_image_free(data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Filter);
    
    if (m_Filter == GL_LINEAR_MIPMAP_LINEAR || m_Filter == GL_LINEAR_MIPMAP_NEAREST ||
        m_Filter == GL_NEAREST_MIPMAP_NEAREST || m_Filter == GL_NEAREST_MIPMAP_LINEAR)
    {
        Use();
        glGenerateMipmap(GL_TEXTURE_2D);
        UnUse();
    }
    
	return true;
}

void Texture::Destory()
{
    if (m_Loaded)
    {
        glDeleteTextures(1, &m_Handle);
        m_Handle = 0;
        m_Width = 0;
        m_Height = 0;
        m_Comp = 0;
        m_Loaded = false;
    }
}

void Texture::Use()
{
    if (m_Loaded)
        glBindTexture(GL_TEXTURE_2D, m_Handle);
}

void Texture::UnUse()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
