#include "ShaderTest.h"

ShaderTest::ShaderTest(Game* game) : State(game), vbo(0), vao(0)
{
}

ShaderTest::~ShaderTest()
{}

bool ShaderTest::Init()
{
	// Points (x,y,z) in clockwise order
	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	// Generate the VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Generate the VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	shader.CompileShader("data/shaders/testShader.vs");
	shader.CompileShader("data/shaders/testShader.fs");
	shader.Link();

	return true;
}

void ShaderTest::Shutdown()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
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
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}