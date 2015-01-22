#ifndef TEXTURE_TEST
#define TEXTURE_TEST

#include "State.h"
#include "Shader.h"
#include "Texture.h"

// Simple test to load a texture and display it on the screen
class TextureTest : public State 
{
private:
	ShaderProgram shader;
    Texture texture;
	GLuint vao;
	glm::mat4 ortho;

public:
	TextureTest(Game* game);
	~TextureTest();

	bool Init();
	void Shutdown();

	void HandleEvents(SDL_Event &e);
	void Update(double dt);
	void Render(double alpha);
};

#endif