#include "VertexBuffer.hpp"
#include <iostream>

VertexBuffer::VertexBuffer(const GLfloat* vertices, const size_t count)
{
	/* Why is a VAO created here?
	 * Performance? Forward compat?
	 * http://stackoverflow.com/questions/5970087/understanding-vertex-array-objects-glgenvertexarrays
	 *
	 * Apparently when you initialize a VAO it saves the parameters set
	 * for the VBOs initialized while it's bound. Neat trick, init once
	 * and draw many times!
	 */
	glGenVertexArrays(1, &_vao_handle);
	glBindVertexArray(_vao_handle);
	glGenBuffers(1, &_vbo_handle);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_handle);
	glBufferData(
			GL_ARRAY_BUFFER,
			count,
			vertices,
			GL_STATIC_DRAW
	);
	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
	);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VertexBuffer::bind() {glBindVertexArray(_vao_handle);}
void VertexBuffer::unbind() {glBindVertexArray(0);}

void VertexBuffer::draw()
{
	bind();
	glDrawArrays(GL_LINE_LOOP, 0, 3);
	unbind();
}
