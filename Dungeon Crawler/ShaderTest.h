#ifndef SHADER_TEST_H
#define SHADER_TEST_H

#include "State.h"
#include "Shader.h"

// Simple test for shaders
// Should have a alternating red/black triangle and a pink square
class ShaderTest : public State
{
private:
	GLuint triangleVbo;
	GLuint squareVbo;
	GLuint triangleVao;
	GLuint squareVao;
	ShaderProgram shader;

public:
	ShaderTest(Game* game);
	~ShaderTest();

	bool Init();
	void Shutdown();

	void HandleEvents(SDL_Event &e);
	void Update(double dt);
	void Render(double alpha);
};

#endif