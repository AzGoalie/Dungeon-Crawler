#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
using namespace std;

namespace ShaderInfo {
	struct shader_file_extension {
		const char *ext;
		Shader::ShaderType type;
	};

	struct shader_file_extension extensions[] =
	{
		{ ".vs", Shader::VERTEX },
		{ ".vert", Shader::VERTEX },
		{ ".gs", Shader::GEOMETRY },
		{ ".geom", Shader::GEOMETRY },
		{ ".fs", Shader::FRAGMENT },
		{ ".frag", Shader::FRAGMENT }
	};
}

ShaderProgram::ShaderProgram() : m_Handle(0), m_Linked(false)
{}

ShaderProgram::~ShaderProgram()
{
	if (m_Handle == 0)
		return;

	GLint numShaders = 0;
	glGetProgramiv(m_Handle, GL_ATTACHED_SHADERS, &numShaders);

	GLuint* shaders = new GLuint[numShaders];
	glGetAttachedShaders(m_Handle, numShaders, NULL, shaders);

	for (int i = 0; i < numShaders; i++)
		glDeleteShader(shaders[i]);

	glDeleteProgram(m_Handle);
	delete[] shaders;
}

string ShaderProgram::GetExt(const char* filename)
{
	string nameStr(filename);

	size_t loc = nameStr.find_last_of('.');
	if (loc != string::npos)
		return nameStr.substr(loc, string::npos);
	return "";
}

bool ShaderProgram::FileExists(const char* filename)
{
	struct stat info;
	int ret = -1;

	ret = stat(filename, &info);
	return ret == 0;
}

bool ShaderProgram::CompileShader(const char* filename)
{
	if (!FileExists(filename))
	{
		cout << "Shader '" << filename << "' does not exist" << endl;
		return false;
	}

	int numExt = sizeof(ShaderInfo::extensions) / sizeof(ShaderInfo::shader_file_extension);

	string ext = GetExt(filename);
	Shader::ShaderType type = Shader::VERTEX;
	bool matchFound = false;
	for (int i = 0; i < numExt; i++)
	{
		if (ext == ShaderInfo::extensions[i].ext)
		{
			matchFound = true;
			type = ShaderInfo::extensions[i].type;
			break;
		}
	}

	if (!matchFound)
	{
		cout << "Unreconized extension: " << ext << endl;
		return false;
	}

	ifstream inFile(filename);
	if (!inFile)
	{
		cout << "Unable to open: " << filename << endl;
		return false;
	}

	stringstream shader;
	shader << inFile.rdbuf();
	inFile.close();

	return CompileShader(shader.str().c_str(), type);
}

bool ShaderProgram::CompileShader(const char* source, Shader::ShaderType type)
{
	if (m_Handle <= 0)
	{
		m_Handle = glCreateProgram();
		if (m_Handle == 0)
		{
			cout << "Unable to create shader program" << endl;
			return false;
		}
	}

	GLuint shaderHandle = glCreateShader(type);
	glShaderSource(shaderHandle, 1, &source, NULL);
	glCompileShader(shaderHandle);

	GLint result;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length = 0;
		string log;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
		if (length > 0)
		{
			char* str = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, str);
			log = str;
			delete[] str;
		}

		cout << "Shader compilation failed\n " << log << endl;
		return false;
	}

	else
	{
		glAttachShader(m_Handle, shaderHandle);
		return true;
	}
}

bool ShaderProgram::Link()
{
	if (m_Linked)
		return true;
	if (m_Handle <= 0)
	{
		cout << "Program has not been compiled" << endl;
		return false;
	}

	glLinkProgram(m_Handle);

	GLint status = 0;
	glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		int length = 0;
		string log;

		glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);

		if (length > 0)
		{
			char* str = new char[length];
			int written = 0;
			glGetProgramInfoLog(m_Handle, length, &written, str);
			log = str;
			delete[] str;
		}
		return false;
	}

	else
	{
		m_Linked = true;
		return true;
	}
}

void ShaderProgram::Use()
{
	if (m_Handle <= 0 || !m_Linked)
		return;
	glUseProgram(m_Handle);
}

void ShaderProgram::UnUse()
{
	glUseProgram(0);
}

bool ShaderProgram::Validate()
{
	if (!IsLinked())
	{
		cout << "Program is not linked" << endl;
		return false;
	}

	GLint status;
	glValidateProgram(m_Handle);
	glGetProgramiv(m_Handle, GL_VALIDATE_STATUS, &status);

	if (GL_FALSE == status) {
		// Store log and return false
		int length = 0;
		string logString;

		glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(m_Handle, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}

		cout << "Program failed to validate\n" << logString;
		return false;
	}
	return true;
}