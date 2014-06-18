#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP
#include <GL/glew.h>

class VertexBuffer
{
private:
	GLuint _vbo_handle;
	GLuint _vao_handle;

public:
	VertexBuffer(const GLfloat* vertices, const size_t count);
	void bind();
	void unbind();
	void draw();
	GLuint vbo_handle() {return _vbo_handle;}
	GLuint vao_handle() {return _vao_handle;}
};


#endif /* end of include guard: VERTEXBUFFER_HPP */
