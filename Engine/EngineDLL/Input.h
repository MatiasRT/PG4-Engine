#pragma once
#include "Window.h"
#include "Exports.h"
#include "GLFW\glfw3.h"

class ENGINEDLL_API Input {
	static Input* instance;

	Window* window;
public:
	Input() {};
	~Input() {};

	bool GetInput(int key);
	void SetWindow(Window * window);
	void PollEvents();

	static Input* Instance() {
		if (instance == NULL) {
			instance = new Input();
		}
		return instance;
	};
};