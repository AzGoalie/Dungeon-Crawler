#include "TextureRegion.h"

TextureRegion::TextureRegion() : m_pTexture(nullptr), m_X(0), m_Y(0), m_Width(0), m_Height(0)
{}

TextureRegion::TextureRegion(Texture* texture, int x, int y, int w, int h)
{
	Load(texture, x, y, w, h);
}

TextureRegion::~TextureRegion()
{
	m_pTexture = nullptr;
	m_X = 0;
	m_Y = 0;
	m_Width = 0;
	m_Height = 0;
}

void TextureRegion::Load(Texture* texture, int x, int y, int w, int h)
{
	m_pTexture = texture;
	m_X = x;
	m_Y = y;
	m_Width = w;
	m_Height = h;
}

void TextureRegion::Use()
{
	if (m_pTexture)
		m_pTexture->Use();
}

void TextureRegion::UnUse()
{
	if (m_pTexture)
		m_pTexture->UnUse();
}