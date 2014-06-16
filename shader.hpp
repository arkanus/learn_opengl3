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
	GLuint handle() {return _handle;}
};

class Program
{
	GLuint _handle;
};

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
