#ifndef SHADER_H
#define SHADER_H

#include "GLHeaders.h"
#include <map>
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
	std::map<std::string, int> m_UniformLocations;

	ShaderProgram(const ShaderProgram& other){};
	ShaderProgram& operator=(const ShaderProgram& other) { return *this; }

	GLint  GetUniformLocation(const char * name);
	std::string GetExt(const char* filename);
	bool FileExists(const char* filename);

public:
	ShaderProgram();
    
    // Will look for all shaders with the file name and add them to the program
    ShaderProgram(std::string filename);
	~ShaderProgram();

    bool CompileShader(const std::string filename);
    bool CompileShader(const std::string, Shader::ShaderType type);

	bool Link();
	void Use();
	void UnUse();

	bool Validate();

	GLuint GetHandle() { return m_Handle; }
	bool IsLinked() { return m_Linked; }

	void   BindAttribLocation(GLuint location, const char * name);
	void   BindFragDataLocation(GLuint location, const char * name);

	void   SetUniform(const char *name, float x, float y, float z);
	void   SetUniform(const char *name, const glm::vec2 & v);
	void   SetUniform(const char *name, const glm::vec3 & v);
	void   SetUniform(const char *name, const glm::vec4 & v);
	void   SetUniform(const char *name, const glm::mat4 & m);
	void   SetUniform(const char *name, const glm::mat3 & m);
	void   SetUniform(const char *name, float val);
	void   SetUniform(const char *name, int val);
	void   SetUniform(const char *name, bool val);
	void   SetUniform(const char *name, GLuint val);

	void   PrintActiveUniforms();
	void   PrintActiveAttribs();

	const char * GetTypeString(GLenum type);
};

#endif