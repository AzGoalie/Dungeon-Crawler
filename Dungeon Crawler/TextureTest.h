#ifndef TEXTURE_TEST
#define TEXTURE_TEST

#include "State.h"
#include "Shader.h"

class TextureTest : public State 
{
private:
	ShaderProgram shader;
	GLuint vao;
	GLuint tex;

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