#ifndef TEXTURE_REGION_H
#define TEXTURE_REGION_H

#include "Texture.h"

class TextureRegion
{
private:
	Texture* m_pTexture;
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

public:

	TextureRegion();
	TextureRegion(Texture* texture, int x, int y, int w, int h);
	~TextureRegion();

	void Load(Texture* texture, int x, int y, int w, int h);

	void Use();
	void UnUse();

	int GetX() { return m_X; }
	int GetY() { return m_Y; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	Texture* GetTexture() { return m_pTexture; }
};

#endif