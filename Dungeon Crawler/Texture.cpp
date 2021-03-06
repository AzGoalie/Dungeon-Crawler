#include "Texture.h"
#include <iostream>
#include <physfs.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::string;

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

Texture::Texture(string filename)
{
    m_Handle = 0;
    m_Wrap = GL_CLAMP_TO_EDGE;
    m_Filter = GL_NEAREST;
    
    m_Width = 0;
    m_Height = 0;
    m_Comp = 0;
    
    m_Loaded = false;
    
    Load(filename);
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

bool Texture::Load(const string filename)
{
	if (m_Loaded)
		Destory();
    
    if (PHYSFS_exists(filename.c_str()) == 0)
    {
        std::cout << "Couldn't find texture: " << filename << std::endl;
        return false;
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenTextures(1, &m_Handle);
    glBindTexture(GL_TEXTURE_2D, m_Handle);
    
    PHYSFS_file* file = PHYSFS_openRead(filename.c_str());
    PHYSFS_sint64 size = PHYSFS_fileLength(file);
    
    unsigned char* image = new unsigned char[size];
    int read = PHYSFS_read(file, image, 1, size);
    PHYSFS_close(file);
    
    if (!image || read != size) {
        std::cout << "Couldn't load texture: " << filename << std::endl;
        return false;
    }
    
    unsigned char* data = stbi_load_from_memory(image, size, &m_Width, &m_Height, &m_Comp, 0);
    
	GLenum format;
	switch (m_Comp)
	{
	case 1:
		format = GL_LUMINANCE;
		break;
	case 2:
		format = GL_LUMINANCE_ALPHA;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		std::cout << "Unknown bpp for texture " << filename << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
    
    delete [] image;
    stbi_image_free(data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Filter);
    
    if (m_Filter == GL_LINEAR_MIPMAP_LINEAR || m_Filter == GL_LINEAR_MIPMAP_NEAREST ||
        m_Filter == GL_NEAREST_MIPMAP_NEAREST || m_Filter == GL_NEAREST_MIPMAP_LINEAR)
    {
        glBindTexture(GL_TEXTURE_2D, m_Handle);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    m_Loaded = true;
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
