#include "Circle.h"

Circle::Circle(Renderer * renderer, float radio, int count):Shape(renderer) {
	triangleCount = count;
	radius = radio;
	angle = 0.0f;
	vtxCount = triangleCount * 3;
	degrees = 360.0f / triangleCount;

	vertex = new float[vtxCount] {};
	glm::vec3 vector;

	for (int i=0; i<vtxCount; i+=3)	{										//Se encarga de cargar el array de vertices
		vector = glm::vec3(cos(angle), sin(angle), 0) * radius;				//Este vector es donde se encuentra el vertice que estoy seteando
		//Estos 3 datos forman un vertice
		vertex[i] = vector.x;
		vertex[i + 1] = vector.y;
		vertex[i + 2] = vector.z;

		angle += degrees * PI / 180.0f;										//Mueve el vertice que va a ser dibujado para formar el triangulo
	}
	SetVertices(vertex, triangleCount);

	SetColorVertices(vertex, triangleCount);
}

Circle::~Circle() {
	delete[] vertex;
}

void Circle::Draw() {
	DrawMesh(GL_TRIANGLE_FAN);
}