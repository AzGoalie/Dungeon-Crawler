#include "ShaderTest.h"
#include <iostream>

ShaderTest::ShaderTest(Game* game) : State(game), triangleVbo(0), squareVbo(0), triangleVao(0), squareVao(0)
{
}

ShaderTest::~ShaderTest()
{}

bool ShaderTest::Init()
{
	// Points (x,y,z) in clockwise order
	GLfloat triangle[] = {
		-0.5f, 0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		-1.0f, -0.5f, 0.0f
	};

	GLfloat square[] = {
		0.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f
	};

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	// Generate the Triangle VBO
	glGenBuffers(1, &triangleVbo);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	// Generate the Square VBO
	glGenBuffers(1, &squareVbo);
	glBindBuffer(GL_ARRAY_BUFFER, squareVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	// Generate the Element buffer
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// Generate the triangle VAO
	glGenVertexArrays(1, &triangleVao);
	glBindVertexArray(triangleVao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Generate the square VAO
	glGenVertexArrays(1, &squareVao);
	glBindVertexArray(squareVao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, squareVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	shader.CompileShader("data/shaders/testShader.vs");
	shader.CompileShader("data/shaders/testShader.fs");
	shader.Link();

	shader.Use();
	shader.SetUniform("triangleColor", 1.0f, 0.0f, 0.0f);
	shader.UnUse();

	glClearColor(.5, .5, .5, 1.0);
	return true;
}

void ShaderTest::Shutdown()
{
	glDeleteBuffers(1, &triangleVbo);
	glDeleteVertexArrays(1, &triangleVao);

	glDeleteBuffers(1, &squareVbo);
	glDeleteVertexArrays(1, &squareVao);
}

void ShaderTest::HandleEvents(SDL_Event &e)
{

}

void ShaderTest::Update(double dt)
{

}

void ShaderTest::Render(double alpha)
{
	shader.Use();
	float time = m_pGame->GetTimeInSec();
	shader.SetUniform("triangleColor", (sin(time * 2.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

	glBindVertexArray(triangleVao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	shader.SetUniform("triangleColor", 1.0f, 0.0f, 1.0f);
	glBindVertexArray(squareVao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	shader.UnUse();
}