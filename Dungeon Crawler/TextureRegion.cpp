#include "TextureRegion.h"

#include <iostream>

TextureRegion::TextureRegion()
{
    m_pTexture = nullptr;
    m_U = 0;
    m_V = 0;
    m_U2 = 0;
    m_V2 = 0;
    m_Width = 0;
    m_Height = 0;
}

TextureRegion::TextureRegion(Texture* texture, int x, int y)
{
    if (texture)
        Set(texture, x, y, texture->GetWidth(), texture->GetHeight());
    else
        std::cout << "Error creating TextureRegion, texture is null" << std::endl;
}

TextureRegion::TextureRegion(Texture* texture, int x, int y, int width, int height)
{
    Set(texture, x, y, width, height);
}

TextureRegion::TextureRegion(Texture* texture, float u, float v, float u2, float v2)
{
    Set(texture, u, v, u2, v2);
}

TextureRegion::~TextureRegion()
{
	m_pTexture = nullptr;
	m_U = 0;
	m_V = 0;
    m_U2 = 0;
    m_V2 = 0;
	m_Width = 0;
	m_Height = 0;
}

void TextureRegion::Set(Texture* texture, int x, int y, int width, int height)
{
    Set(texture, x / (float)texture->GetWidth(),
        y / (float)texture->GetHeight(),
        (x + width) / (float)texture->GetWidth(),
        (y + height) / (float)texture->GetHeight());
    m_Width = width;
    m_Height = height;
}

void TextureRegion::Set(Texture* texture, float u, float v, float u2, float v2)
{
    m_pTexture = texture;
    m_U = u;
    m_V = v;
    m_U2 = u2;
    m_V2 = v2;
    m_Width = abs(u2 - u) * texture->GetWidth();
    m_Height = abs(v2 - v) * texture->GetHeight();
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

void TextureRegion::Flip(bool x, bool y)
{
    if (x)
    {
        float temp = m_U;
        m_U = m_U2;
        m_U2 = temp;
    }
    
    if (y)
    {
        float temp = m_V;
        m_V = m_V2;
        m_V2 = temp;
    }
}

int TextureRegion::GetRegionX()
{
    if (m_pTexture)
        return m_U * m_pTexture->GetWidth();
    else
        return 0;
}

int TextureRegion::GetRegionY()
{
    if (m_pTexture)
        return m_V * m_pTexture->GetHeight();
    else
        return 0;
}