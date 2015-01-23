#include "Shader.h"

#include <iostream>
#include <physfs.h>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::ifstream;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;


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

ShaderProgram::ShaderProgram(string filename)
{
    m_Handle = 0;
    m_Linked = false;
    
    // Search filepath for any shaders with same name
    int numExt = sizeof(ShaderInfo::extensions) / sizeof(ShaderInfo::shader_file_extension);
    bool found = false;
    for (int i = 0; i < numExt; i++)
    {
        string path = filename;
        path.append(ShaderInfo::extensions[i].ext);
        
        if (FileExists(path.c_str()))
        {
            found = true;
            if (!CompileShader(path))
                cout << "Failed to compile shader: " << path << endl;
        }
    }
    
	if (found)
	{
		if (!Link())
			cout << "failed to link shader: " << filename << endl;
	}
    else
        cout << "failed to load shader: " << filename << endl;
    
}

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
	if (PHYSFS_exists(filename) == 0)
        return false;
    else
        return true;
}

bool ShaderProgram::CompileShader(const string filename)
{
	if (!FileExists(filename.c_str()))
	{
		cout << "Shader '" << filename << "' does not exist" << endl;
		return false;
	}

	int numExt = sizeof(ShaderInfo::extensions) / sizeof(ShaderInfo::shader_file_extension);

	string ext = GetExt(filename.c_str());
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

    PHYSFS_file* file = PHYSFS_openRead(filename.c_str());
    PHYSFS_sint64 size = PHYSFS_fileLength(file);
    
    char* src = new char[size+1];
    int read = PHYSFS_read(file, src, 1, size);
    src[size] = '\0';
    PHYSFS_close(file);

	if (!src || read != size)
	{
		cout << "Unable to read: " << filename << endl;
		return false;
	}
    
	bool result = CompileShader(src, type);
    delete [] src;
    
    return result;
}

bool ShaderProgram::CompileShader(const string source, Shader::ShaderType type)
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
    const char* src = source.c_str();
	glShaderSource(shaderHandle, 1, &src, NULL);
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

		cout << "Shader compilation failed for " << source << endl << log << endl;
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

	if (GL_FALSE == status) 
	{
		int length = 0;
		string logString;

		glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) 
		{
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

void ShaderProgram::SetUniform(const char *name, float x, float y, float z)
{
	GLint loc = GetUniformLocation(name);
	glUniform3f(loc, x, y, z);
}

void ShaderProgram::SetUniform(const char *name, const vec3 & v)
{
	this->SetUniform(name, v.x, v.y, v.z);
}

void ShaderProgram::SetUniform(const char *name, const vec4 & v)
{
	GLint loc = GetUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::SetUniform(const char *name, const vec2 & v)
{
	GLint loc = GetUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::SetUniform(const char *name, const mat4 & m)
{
	GLint loc = GetUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void ShaderProgram::SetUniform(const char *name, const mat3 & m)
{
	GLint loc = GetUniformLocation(name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void ShaderProgram::SetUniform(const char *name, float val)
{
	GLint loc = GetUniformLocation(name);
	glUniform1f(loc, val);
}

void ShaderProgram::SetUniform(const char *name, int val)
{
	GLint loc = GetUniformLocation(name);
	glUniform1i(loc, val);
}

void ShaderProgram::SetUniform(const char *name, GLuint val)
{
	GLint loc = GetUniformLocation(name);
	glUniform1ui(loc, val);
}

void ShaderProgram::SetUniform(const char *name, bool val)
{
	int loc = GetUniformLocation(name);
	glUniform1i(loc, val);
}

void ShaderProgram::BindAttribLocation(GLuint location, const char * name)
{
	glBindAttribLocation(m_Handle, location, name);
}

void ShaderProgram::BindFragDataLocation(GLuint location, const char * name)
{
	glBindFragDataLocation(m_Handle, location, name);
}

int ShaderProgram::GetUniformLocation(const char * name)
{
	std::map<string, int>::iterator pos;
	pos = m_UniformLocations.find(name);

	if (pos == m_UniformLocations.end())
		m_UniformLocations[name] = glGetUniformLocation(m_Handle, name);

	return m_UniformLocations[name];
}


const char * ShaderProgram::GetTypeString(GLenum type) 
{
	switch (type) 
	{
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_DOUBLE:
		return "double";
	case GL_INT:
		return "int";
	case GL_UNSIGNED_INT:
		return "unsigned int";
	case GL_BOOL:
		return "bool";
	case GL_FLOAT_MAT2:
		return "mat2";
	case GL_FLOAT_MAT3:
		return "mat3";
	case GL_FLOAT_MAT4:
		return "mat4";
	default:
		return "?";
	}
}

void ShaderProgram::PrintActiveUniforms() 
{
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(m_Handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	printf("Active uniforms:\n");
	for (int i = 0; i < numUniforms; ++i) 
	{
		GLint results[4];
		glGetProgramResourceiv(m_Handle, GL_UNIFORM, i, 4, properties, 4, NULL, results);

		if (results[3] != -1) continue;  // Skip uniforms in blocks 
		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(m_Handle, GL_UNIFORM, i, nameBufSize, NULL, name);
		printf("%-5d %s (%s)\n", results[2], name, GetTypeString(results[1]));
		delete[] name;
	}
}

void ShaderProgram::PrintActiveAttribs() 
{
	GLint numAttribs;
	glGetProgramInterfaceiv(m_Handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	printf("Active attributes:\n");
	for (int i = 0; i < numAttribs; ++i) 
	{
		GLint results[3];
		glGetProgramResourceiv(m_Handle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(m_Handle, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name);
		printf("%-5d %s (%s)\n", results[2], name, GetTypeString(results[1]));
		delete[] name;
	}
}