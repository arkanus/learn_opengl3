#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdio.h>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
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

#define VERTEX_SHADER " \
  #version 130\n \
  in vec4 position; \
  in vec4 color; \
  smooth out vec4 vColor; \
  void main() { \
  gl_Position = position; \
    vColor = color; \
  }"

#define FRAGMENT_SHADER " \
  #version 130\n \
  smooth in vec4 vColor; \
  void main() { \
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); \
  }"

GLuint CreateShader(GLenum shaderType, const char* shaderSource)
{
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, (const GLchar **)&shaderSource, NULL);
  glCompileShader(shader);
  CHECK_GL_STATUS(Shader, shader, GL_COMPILE_STATUS);
  return shader;
}

class App
{
protected:
	sf::Window* _window;
	sf::Clock clock;
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint shader_program;

public:
	App(const int width, const int height, const char* title)
	{
		_window = new sf::Window(
				sf::VideoMode(width, height, 32),
				title,
				sf::Style::Close);
		init_gl();
		init_glew();
	}

	//TODO Move to Renderer
	void init_gl()
	{
		glClearDepth(1.f);
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.f, 1.f, 1.f, 500.f);
	}

	void create_shader_program()
	{
		GLuint vertex = CreateShader(GL_VERTEX_SHADER, VERTEX_SHADER);
		GLuint fragment = CreateShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex);
		glAttachShader(shader_program, fragment);
		glLinkProgram(shader_program);
		CHECK_GL_STATUS(Program, shader_program, GL_LINK_STATUS);
	}

	bool init_glew()
	{
		if (glewInit() != GLEW_OK) {
			std::cout << "Failed to init GLEW" << std::endl;
			return 0;
		}
		return 1;
	}

	void render_frame()
	{
		_window->SetActive();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();
		_window->Display();
	}

	void init_scene()
	{
		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);
		static const GLfloat vertex_data[] = {
			-1.f, -1.f, 0.f,
			 1.f, -1.f, 0.f,
			 0.f,  1.f, 0.f
		};
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data),
		             vertex_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		create_shader_program();
	}

	void draw()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -1.f);
		glRotatef(clock.GetElapsedTime() * 50, 1.f, 0.f, 0.f);
		glRotatef(clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);
		glRotatef(clock.GetElapsedTime() * 90, 0.f, 0.f, 1.f);
		glColor3f(1.f, 1.f, 1.f);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
		glUseProgram(shader_program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
	}

	int handle_events()
	{
		sf::Event event;
		while (_window->GetEvent(event)) {
			if (event.Type == sf::Event::Closed)
			{
				std::cout << "Exiting app" << std::endl;
				return 0;
			}
		}
		return 1;
	}

	int start()
	{
		int code = 1;
		std::cout << "Starting app" << std::endl;
		while (code = handle_events()) {
			render_frame();
		}
		return code;
	}
};

int main(int argc, char** argv)
{
	App app(800, 600, "Stupid OpenGL demo");
	app.init_scene();
	return app.start();
}
