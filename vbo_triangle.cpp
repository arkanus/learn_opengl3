#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdio.h>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
#include "shader.hpp"
#include "VertexBuffer.hpp"

class App
{
protected:
	sf::Window* _window;
	sf::Clock clock;
	GLuint vertex_array;
	GLuint vertex_buffer;

	Program* _program;
	VertexBuffer* _vbo;

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
		static const GLfloat vertex_data[] = {
			-1.f, -1.f, 0.f,
			1.f, -1.f, 0.f,
			0.f,  1.f, 0.f
		};
		std::cout << "vbo data size: " << sizeof(vertex_data) << std::endl;
		_vbo = new VertexBuffer(vertex_data, sizeof(vertex_data));
		_program = new Program("vertex.glsl", "fragment.glsl");

	}

	void draw()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -1.f);
		// glRotatef(clock.GetElapsedTime() * 50, 1.f, 0.f, 0.f);
		// glRotatef(clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);
		// glRotatef(clock.GetElapsedTime() * 90, 0.f, 0.f, 1.f);
		glColor3f(1.f, 1.f, 1.f);
		_program->use();
		_vbo->draw();
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
