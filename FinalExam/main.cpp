#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "irrKlang/include/irrKlang.h"

#include "Window.h"

#include <iostream>

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	cx = xpos;
	cy = ypos;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lbutton_down = true;
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (GLFW_PRESS == action)
			rbutton_down = true;
		else if (GLFW_RELEASE == action)
			rbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (GLFW_PRESS == action)
			mbutton_down = true;
		else if (GLFW_RELEASE == action)
			mbutton_down = false;
	}
}

static void mouseDragging(Window& win, double width, double height)
{
	if (lbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win.rotate(fractionChangeX, fractionChangeY);
	}
	else if (mbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win.zoom(fractionChangeY);
	}
	else if (rbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win.translate(-fractionChangeX, -fractionChangeY, 1);
	}
	m_lastMouseX = cx;
	m_lastMouseY = cy;
}

int main()
{
	int width = 800;
	int height = 800;
	GLFWwindow *gl_win_;

	/* Initialize the library */
	if (!glfwInit())
		throw std::runtime_error("OpenGL initialization failed. Aborting program...");

	//Defines OpenGL versions and profile used
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	gl_win_ = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
	if (!gl_win_)
	{
		glfwTerminate();
		throw std::runtime_error("Couldn't create window. Terminating OpenGL...");
	}

	/* Make the window's context current and initializes glew */
	glfwMakeContextCurrent(gl_win_);
	glewExperimental = GL_TRUE;
	if (GLenum{ glewInit() } != GLEW_OK)
	{
		glfwTerminate();
		throw std::runtime_error("glewInit failed. Terminating program...");
	}

	glfwSwapInterval(1);  //enable vsync

	glfwSetMouseButtonCallback(gl_win_, mouse_button_callback);
	glfwSetCursorPosCallback(gl_win_, cursor_pos_callback);
	glfwSetKeyCallback(gl_win_, key_callback);

	const char* glsl_version = "#version 430";
	
	std::cout << "OpenGL " << glGetString(GL_VERSION) << " GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	glfwMakeContextCurrent(gl_win_);

	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	if (!engine)
		return 1;

	Window win{ (uint16_t)width, (uint16_t)height };

	engine->play2D("../warm.ogg", true);
	std::cout << "Music: Warm by A.L.I.S.O.N" << std::endl;

	while (!glfwWindowShouldClose(gl_win_))
	{
		glEnable(GL_DEPTH_TEST);  //enable depth testing

		glfwGetFramebufferSize(gl_win_, &width, &height);
		glViewport(0, 0, width, height);  //viewport setting

		win.tick();

		/* Swap front and back buffers */
		glfwSwapBuffers(gl_win_);
		/* Poll for and process events */
		glfwPollEvents();

		mouseDragging(win, width, height);
	}

	engine->drop();

	glfwDestroyWindow(gl_win_);
	glfwTerminate();

	return 0;
}