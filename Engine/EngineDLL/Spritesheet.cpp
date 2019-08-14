#include "Spritesheet.h"

Spritesheet::Spritesheet(int tW, int tH, int col, int rows) {
	//uvList = new list<float*>();															// Creo una lista de punteros a floats, donde se van a guardar los uv´s de la textura
	uvVector = new vector<float*>();														// Creo un vector de punteros a floats, donde se van a guardar los uv´s de la textura

	int countFrames = rows * col;															// Multiplico la cantidad de columnas y de filas que tiene el sprite para poder dibujar el correcto
	float wFrame = 1.0f / col;																// ancho de cada sprite dentro de la imagen.
	float hFrame = 1.0f / rows;																// alto de cada sprite dentro de la imagen.
	float x;
	float y;

	for (int i = 0; i < countFrames; i++) {													
		//x = (i % countFrames) * wFrame;
		//y = (i / countFrames) * hFrame;
		x = (i % col) * wFrame;
		y = (i / col) * hFrame;

		uvPositions = new float[8]{															// Llenamos el arreglo de uvs
			x / tW, 1 - ((y + hFrame) / tH),												// Este seria el punto arriba a la izquierda
			x / tW, 1 - y / tH,																// Este arriba a la derecha
			(x + wFrame) / tW , 1 - ((y + hFrame) / tH),									// Este abajo a la izquierda
			(x + wFrame) / tW, 1 - y / tH													// Y este abajo a la derecha
		};
		//uvList->push_back(uvPositions);													// Le pasamos a la lista de uv los puntos guardados en el arreglo
		uvVector->push_back(uvPositions);													// Le pasamos al vector de uv los puntos guardados en el arreglo
	}
}

float * Spritesheet::SetSpritesheet(int frame) {											// Recibe de la animacion el frame en el que se encuentra
	int fr = 0;
	for (vector<float*>::iterator i = uvVector->begin(); i != uvVector->end(); i++) {		
		if (fr == frame)																	
			return *i;																		// Devuelve el uvVector para que se vaya actualizando el sprite
		else fr++;
	}
}