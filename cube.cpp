#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

class App
{
protected:
	sf::Window* _window;
	sf::Clock clock;
	
public:
	App(const int height, const int width, const char* title)
	{
		_window = new sf::Window(
				sf::VideoMode(width, height, 32),
				title,
				sf::Style::Close);
		init_gl();
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

	void render_frame()
	{
		_window->SetActive();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();
		_window->Display();
	}

	void draw()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -200.f);
		glRotatef(clock.GetElapsedTime() * 50, 1.f, 0.f, 0.f);
		glRotatef(clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);
		glRotatef(clock.GetElapsedTime() * 90, 0.f, 0.f, 1.f);
		glBegin(GL_QUADS);

			glVertex3f(-50.f, -50.f, -50.f);
			glVertex3f(-50.f,  50.f, -50.f);
			glVertex3f( 50.f,  50.f, -50.f);
			glVertex3f( 50.f, -50.f, -50.f);

			glVertex3f(-50.f, -50.f, 50.f);
			glVertex3f(-50.f,  50.f, 50.f);
			glVertex3f( 50.f,  50.f, 50.f);
			glVertex3f( 50.f, -50.f, 50.f);

			glVertex3f(-50.f, -50.f, -50.f);
			glVertex3f(-50.f,  50.f, -50.f);
			glVertex3f(-50.f,  50.f,  50.f);
			glVertex3f(-50.f, -50.f,  50.f);

			glVertex3f(50.f, -50.f, -50.f);
			glVertex3f(50.f,  50.f, -50.f);
			glVertex3f(50.f,  50.f,  50.f);
			glVertex3f(50.f, -50.f,  50.f);

			glVertex3f(-50.f, -50.f,  50.f);
			glVertex3f(-50.f, -50.f, -50.f);
			glVertex3f( 50.f, -50.f, -50.f);
			glVertex3f( 50.f, -50.f,  50.f);

			glVertex3f(-50.f, 50.f,  50.f);
			glVertex3f(-50.f, 50.f, -50.f);
			glVertex3f( 50.f, 50.f, -50.f);
			glVertex3f( 50.f, 50.f,  50.f);

		glEnd();
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
		std::cout << "Starting app" << std::endl;
		while (handle_events()) {
			render_frame();
		}
	}
};

int main(int argc, char** argv)
{
	App app(800, 600, "Stupid OpenGL demo");
	app.start();
}
