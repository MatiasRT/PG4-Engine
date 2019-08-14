#include "Rectangle.h"

Rectangle::Rectangle(Renderer* renderer) : Shape(renderer) {

	vertex = new float[12]{													//Este arreglo representa los cuatro vertices que tiene un rectangulo (Acordate que X es a la derecha, e Y es arriba(dedos))
		-1.0f, -1.0f, 0.0f,													//Vertice 1: Lo estamos seteando en el (-1,-1) de la pantalla, que seria la esquina inferior izquierda de la pantalla
		1.0f, -1.0f, 0.0f,													//Vertice 2: Lo estamos seteando en el (1,-1) de la pantalla, que seria la esquina inferior derecha de la pantalla
		-1.0f, 1.0f, 0.0f,													//Vertice 3: Lo estamos seteando en el (-1, 1) de la pantalla, que seria la esquina superior izquierda de la pantalla
		1.0f, 1.0f, 0.0f													//Vertice 4: Lo estamos seteando en el (1, 1) de la pantalla, que seria la esquina superior derecha de la pantalla
	};

	SetVertices(vertex, 4);

	colorVertex = new float[12]{
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f
	};

	SetColorVertices(colorVertex, 4);
}

Rectangle::~Rectangle() {
	delete[] vertex;
	delete[] colorVertex;
}

void Rectangle::Draw() {
	DrawMesh(GL_TRIANGLE_STRIP);
}