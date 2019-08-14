#include "Animation.h"

Animation::Animation(int col, int rows) {
	sprite = new Spritesheet(1, 1, col, rows);
}

void Animation::SetAnimation(int firstF, int finalF, float timeF) {							// Recibimos el primer frame de la imagen, el ultimo, y el tiempo en el que queremos que se pase de frame en la imagen
	if (firstF >= 0 && finalF < sprite->GetSize() && timeF > 0.0f) {						// Verifica que los parametros que le mandamos sean correctos, sino se romperia la animaxion.
		firstFrame = firstF;																// Nos guardamos el valor que queremos que sea el primer frame
		finalFrame = finalF;																// Nos guardamos el valor en donde termina la animacion que estamos usando
		timeFrame = timeF;																	// Nos guardamos el tiempo en el que queremos que se ejecute, la velocidad en la que cambia la imagen.
		currentTime = 0.0f;																		
		currentFrame = firstFrame;															// Igualamos el primer frame a el frame en el que estamos.
	}
}

float * Animation::UpdateAnimation(float time) {											// Aca manejamos la logica de la animacion
	currentTime += time;																	// Sumamos constantemente al tiempo 
	if (currentTime > timeFrame) {															// Entra si el current time es mayor al la velicudad en la que cambia la imagen
		currentTime = 0.0f;																	// Resetea el current time
		if (currentFrame < finalFrame)														// Si el primer frame de la animacion (pasado por parametro) es menor que el ultimo frame sigue aumentando 
			currentFrame++;																	// hasta llegar a el ultimo frame de la animacion.
		else	
			currentFrame = firstFrame;														// Cuando llega a ser igual al ultimo frame de esa animacion, vuelve a ser reseteado al primer frame
	}																						// y sigue loopeando entre el primer frame y el ultimo, para poder hacer que la animacion se mueva.
	return sprite->SetSpritesheet(currentFrame);											// Le pasamos al spritesheet el frame en el que nos encontramos de la animacion.
}