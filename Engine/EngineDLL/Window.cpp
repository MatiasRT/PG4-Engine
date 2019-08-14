#include "Window.h"
#include <GLFW\glfw3.h>

bool Window::Start(int w, int h, const char* n) {
	
	if (!glfwInit())												//Inicializar GLFW
		return -1;

	/*Create a window and create its OpenGL context*/
	window = glfwCreateWindow(w, h, n, NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window");
		glfwTerminate();
		return false;
	}

	return true;
}

bool Window::Stop() {
	if (window != NULL)
		glfwDestroyWindow((GLFWwindow*)window);
	
	window = NULL;
	
	glfwTerminate();

	return true;
}

bool Window::ShouldClose() {
	if (window)
		return glfwWindowShouldClose((GLFWwindow*)window);

	return true;
}

void Window::PollEvents() {
	glfwPollEvents();
}

void* Window::GetWindow() {
	return window;
}