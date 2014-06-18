#include <GL/glew.h>
#include <string>
#include <iostream>

#define CHECK_GL_STATUS(T,O,S) { \
  GLint status; \
  glGet##T##iv(O, S, &status); \
  if (GL_FALSE == status) { \
    GLint logLength; \
    glGet##T##iv(O, GL_INFO_LOG_LENGTH, &logLength); \
    char *infoLog = (char*)malloc(logLength * sizeof(char)); \
    glGet##T##InfoLog(O, logLength, NULL, infoLog); \
    fprintf(stderr, "%d: %d, %s\n", __LINE__, S, infoLog); \
    free(infoLog); \
  } \
}

template <GLuint type>
class Shader
{
protected:
	GLuint _handle;

	GLuint create(const char* code);

public:
	Shader(const char* filename);
	Shader(std::string code);
	GLuint handle() const {return _handle;}
};

typedef Shader<GL_VERTEX_SHADER> VertexShader;
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;


class Program
{
protected:
	GLuint _handle;

	GLuint link(GLuint vertex, GLuint fragment);

public:
	Program(const VertexShader& vs, const FragmentShader& fs);
	Program(const char* vs_code, const char* fs_code);
	GLuint handle() const {return _handle;}
	void use() {glUseProgram(_handle);}
};


/**
 * Implementations
 **/

/** Shader **/

std::string read_file(const char* filename)
{
	std::ifstream in(filename, std::ios::in|std::ios::binary);
	if(in)
	{
		std::string content;
		in.seekg(0);
		in.seekg(0, std::ios::end);
		content.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&content[0], content.size());
		in.close();
		return(content);
	}
	throw(errno);
}

template <GLuint type>
GLuint Shader<type>::create(const char* code)
{
	_handle = glCreateShader(type);
	glShaderSource(_handle, 1, (const GLchar**)&code, NULL);
	glCompileShader(_handle);
	CHECK_GL_STATUS(Shader, _handle, GL_COMPILE_STATUS);
	return _handle;
}

template <GLuint type>
Shader<type>::Shader(const char* filename)
{
	std::string code = read_file(filename);
	std::cout << "loading from file :" << filename << std::endl;
	_handle = create(code.c_str());
}

template <GLuint type>
Shader<type>::Shader(std::string code)
{
	std::cout << "loading from code";
	_handle = create(code.c_str());
}

/** Program **/
GLuint Program::link(const GLuint vertex, const GLuint fragment)
{
	_handle = glCreateProgram();
	if (vertex != 0) {
		glAttachShader(_handle, vertex);
	}
	if (fragment != 0) {
		glAttachShader(_handle, fragment);
	}
	glLinkProgram(_handle);
	CHECK_GL_STATUS(Program, _handle, GL_LINK_STATUS);
}

Program::Program(const VertexShader& vs, const FragmentShader& fs)
{
	this->link(vs.handle(), fs.handle());
}

Program::Program(const char* vs_file, const char* fs_file)
{
	VertexShader vs(vs_file);
	FragmentShader fs(fs_file);
	this->link(vs.handle(), fs.handle());
}
