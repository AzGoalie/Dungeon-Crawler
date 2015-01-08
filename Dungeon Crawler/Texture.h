#ifndef TEXTURE_H
#define TEXTURE_H

#include "GLHeaders.h"

class Texture
{
private:
	GLuint m_Handle;
	GLint m_Wrapping;
	GLint m_Filter;

	int m_Width;
	int m_Height;

	bool m_Loaded;
	bool m_Bound;
public:
	Texture();
	~Texture();

	bool Load(const char* filename);
	void Destory();

	void Bind();
	void Unbind();
};

#endif