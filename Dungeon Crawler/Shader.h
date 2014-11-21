#ifndef SHADER_H
#define SHADER_H

#include "GLHeaders.h"
#include <string>

namespace Shader
{
	enum ShaderType
	{
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER
	};
}

// Baised on OpenGL Shader Cookbook
class ShaderProgram
{
protected:
	GLuint m_Handle;
	bool m_Linked;

	ShaderProgram(const ShaderProgram& other){};
	ShaderProgram& operator=(const ShaderProgram& other) { return *this; }

	std::string GetExt(const char* filename);
	bool FileExists(const char* filename);
public:
	ShaderProgram();
	~ShaderProgram();

	bool CompileShader(const char* filename);
	bool CompileShader(const char* source, Shader::ShaderType type);

	bool Link();
	void Use();
	void UnUse();

	bool Validate();

	GLuint GetHandle() { return m_Handle; }
	bool IsLinked() { return m_Linked; }
};

#endif