#pragma once
#include "Exports.h"
#include "Definitions.h"
#include "Renderer.h"
#ifndef COLLIDER_H
#define COLLIDER_H

class ENGINEDLL_API Collider {
	Renderer * renderer;
	glm::vec3 collider[8];
public:
	Collider(Renderer * render);
	~Collider() { };

	glm::vec3 GetVertices(int index);
	float * GetVertives();
	void SetVertices(glm::vec3 col[8]);
	void SetVertices(int index, glm::vec3 pos);
};
#endif