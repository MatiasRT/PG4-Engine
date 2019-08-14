#include "Input.h"

Input* Input::instance = NULL;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

bool Input::GetInput(int key) {
	int keyPress = glfwGetKey((GLFWwindow*)window->GetWindow(), key);
	if (keyPress == GLFW_PRESS)
		return true;
	return false;
}

void Input::SetWindow(Window * window) {
	this->window = window;
	glfwSetKeyCallback((GLFWwindow*)this->window->GetWindow(), KeyCallback);
}

void Input::PollEvents() {
	glfwPollEvents();
}