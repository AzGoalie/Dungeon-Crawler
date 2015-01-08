#include "Texture.h"
#include <iostream>

Texture::Texture() {
	m_Handle = 0;
	m_Wrapping = GL_CLAMP_TO_EDGE;
	m_Filter = GL_NEAREST_MIPMAP_NEAREST;

	m_Width = 0;
	m_Height = 0;

	m_Loaded = false;
	m_Bound = false;
}

Texture::~Texture() {
	if (m_Loaded)
		Destory();
}

bool Texture::Load(const char* filename) {
	if (m_Loaded)
		Destory();
	return true;
}

void Texture::Destory()
{}

void Texture::Bind()
{}

void Texture::Unbind()
{}
