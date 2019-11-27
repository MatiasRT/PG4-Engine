#include "Collider.h"

Collider::Collider(Renderer * render) {
	renderer = render;
}

glm::vec3 Collider::GetVertices(int index) {													// Obtengo los puntos del collider segun la posicion global
	glm::vec4 globalRotPosition(collider[index].x, collider[index].y, collider[index].z, 1.0f);			

	globalRotPosition = renderer->GetWorldMatrix() * globalRotPosition;									

	return (glm::vec3)globalRotPosition;																
}

float * Collider::GetVertives() {
	float * result = new float[8 * 3];
	int cantVetex = 0;
	for (size_t i = 0; i < 8; i++)
	{
		glm::vec3 cube = GetVertices(i);
		result[i] = cube.x;
		result[i + 1] = cube.y;
		result[i + 2] = cube.z;
		cantVetex += 3;
	}
	return result;
}

void Collider::SetVertices(glm::vec3 col[8]) {
	for (int i = 0; i < 8; i++)
		collider[i] = col[i];
}

void Collider::SetVertices(int index, glm::vec3 pos) {
	collider[index].x = pos.x; collider[index].y = pos.y; collider[index].z, pos.z;
}