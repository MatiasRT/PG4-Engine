#include "Collider.h"

Collider::Collider(Renderer * render) {
	renderer = render;
}

glm::vec3 Collider::GetVertices(int index) {													// Obtengo los puntos del collider segun la posicion global
	glm::vec4 globalRotPosition(collider[index].x, collider[index].y, collider[index].z, 1.0f);			

	globalRotPosition = renderer->GetWorldMatrix() * globalRotPosition;									

	return (glm::vec3)globalRotPosition;																
}

void Collider::SetVertices(glm::vec3 col[8]) {
	for (int i = 0; i < 8; i++)
		collider[i] = col[i];
}