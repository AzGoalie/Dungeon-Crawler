#ifndef TEXTURE_REGION_H
#define TEXTURE_REGION_H

#include "Texture.h"

class TextureRegion
{
private:
	Texture* m_pTexture;
	float m_U;
	float m_V;
	float m_U2;
	float m_V2;
    
    int m_Width;
    int m_Height;

public:

	TextureRegion();
    TextureRegion(Texture* texture);
    TextureRegion(Texture* texture, int x, int y);
	TextureRegion(Texture* texture, int x, int y, int w, int h);
    TextureRegion(Texture* texture, float u, float v, float u2, float v2);
    
	~TextureRegion();

	void Set(Texture* texture, int x, int y, int w, int h);
    void Set(Texture* texture, float u, float v, float u2, float v2);
    
	void Use();
	void UnUse();

    void Flip(bool x, bool y);
    
    int GetRegionX();
    int GetRegionY();
    
    float GetU() { return m_U; }
    float GetV() { return m_V; }
    float GetU2() { return m_U2; }
    float GetV2() { return m_V2; }
    
    void SetU(float u) { m_U = u; }
    void SetV(float v) { m_V = v; }
    void SetU2(float u2) { m_U2 = u2; }
    void SetV2(float v2) { m_V2 = v2; }
    
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
    
    void SetTexture(Texture* texture) { m_pTexture = texture; }
	Texture* GetTexture() { return m_pTexture; }
};

#endif