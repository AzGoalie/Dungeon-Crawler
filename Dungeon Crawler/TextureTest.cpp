#include "TextureTest.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureTest::TextureTest(Game* game) : State(game), vao(0), tex(0) 
{}

TextureTest::~TextureTest()
{}

bool TextureTest::Init()
{
	GLfloat square[] = {
//		 x		y	 z		t	s
		-0.5f, 0.5f, 0.0f, 0.0, 0.0,
		0.5f, 0.5f, 0.0f, 1.0, 0.0,
		0.5f, -0.5f, 0.0f, 1.0, 1.0,
		-0.5f, -0.5f, 0.0f, 0.0, 1.0
	};

	GLuint ele[] = {
		0, 1, 2,
		2, 3, 0
	};

	// Generate the VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	// Generate the EBO
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ele), ele, GL_STATIC_DRAW);

	// Generate the VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);

	// Texcoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	// Create the texture
	int w, h, comp;
	unsigned char* data;
	glGenTextures(1, &tex);
	FILE* image = fopen("data/PlanetCutePNG/Brown Block.png", "rb");
	if (!image)
		return false;
	data = stbi_load_from_file(image, &w, &h, &comp, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	fclose(image);
	stbi_image_free(data);

	// Texture Filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Create the shader
	shader.CompileShader("data/shaders/testTexture.vs");
	shader.CompileShader("data/shaders/testTexture.fs");
	shader.Link();

	shader.Use();
	shader.SetUniform("triangleColor", 1.0, 0.0, 0.0);

	// Enable blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.3, 0.3, 0.3, 1.0);
	return true;
}

void TextureTest::Shutdown()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &tex);
}

void TextureTest::HandleEvents(SDL_Event &e)
{}

void TextureTest::Update(double dt)
{}

void TextureTest::Render(double alpha)
{
	shader.Use();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	shader.UnUse();
}