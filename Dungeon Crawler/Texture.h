#ifndef TEXTURE_H
#define TEXTURE_H

#include "GLHeaders.h"
#include <string>

class Texture
{
private:
	GLuint m_Handle;    // GL Texture ID
	GLint m_Wrap;       // Wrapping filter, should make seperate for s/t
	GLint m_Filter;     // Texture filter, should make seprate for min/max

    // Texture info
    int m_Width;
	int m_Height;
    int m_Comp;     // bbp

	bool m_Loaded;
public:
	Texture();
    Texture(std::string filename);
	~Texture();

    void SetWrap(GLuint wrap);
    void SetFilter(GLuint filter);
    GLuint GetWrap() { return m_Wrap; }
    GLuint GetFilter() { return m_Filter; }
    
    int GetWidth() { return m_Width; }
    int GetHeight() { return m_Height; }
    int GetComp() { return m_Comp; }
    
    int GetHandle() { return m_Handle; }
    bool IsLoaded() { return m_Loaded; }
    
    bool Load(std::string filename);
	void Destory();

	void Use();
	void UnUse();
};

#endif