#include "Tilemap.h"

Tilemap::Tilemap(const char* filepath, int winWidth, int winHeight, Material * mat, Renderer * rend) {		// Cargamos el archivo
	
	render = rend;																			// Inicializamos variables
	material = mat;
	scrollX = 0;
	scrollY = 0;
	levelHeight = 1;
	levelWidth = 1;
	lastPosX = 0;
	lastPosY = 0;
	LastCameraPos = glm::vec3(0, 0, 0);
	CurrentCameraPos = glm::vec3(0, 0, 0);
	DeltaCameraPos = glm::vec3(0, 0, 0);

	string buffer;
	ifstream tilemap(filepath);																// Abrimos el archivo
	
	getline(tilemap, buffer);																// Extraemos la primera linea del archivo

	for (int i = 0; i < buffer.length(); i++)
		if (buffer[i] == ',')
			levelWidth++;																	// Saco la cantidad de columnas
	
	while (getline(tilemap, buffer)) {
		levelHeight++;																		// Saco la cantidad de filas
	}

	tilemap.clear();																		// Con esto eliminamos el estado de eof del stream, para que podamos seguir utilizandolo
	tilemap.seekg(0, std::ios::beg);														// Cambiamos la posicion del cursor en el archivo (primer parametro es el salto, y el segundo es el punto de partida)

	level = new vector<vector<int>*>(levelWidth);											// Inicializamos el vector con la cantidad de columnas totales
	for (int i = 0; i < levelWidth; i++)
		level->at(i) = new vector<int>(levelHeight);										// Le agrego las filas al vector
		
	if (tilemap.is_open()) {																// Preguntamos si el archivo esta abierto
		for (int i = 0; i < levelWidth; i++) {												// Recorremos por la cantidad de columnas, por el ancho

			getline(tilemap, buffer);														// Obtengo la primera linea
			int levelW = 0;

			for (int j = 0; j < buffer.length(); j++) {										// Los archivos CSV tienen los valores 0 (hay informacion) y -1 (esta vacio)
				if (buffer[j] == '0') {														// Si la linea que leyó contiene un cero es que hay informacion
					level->at(i)->at(levelW) = 1;											// Le mandamos al vector que en esa posicion hay un 1
					levelW++;																// Pasamos al siguiente caracter
				}
				else if (buffer[j] == '1') {												// Si la linea que leyó contiene un 1 (no importa el signo) es que esta vacio
					level->at(i)->at(levelW) = 0;											// Le asignamos un 0 en esa posicion del vector
					levelW++;																// Pasamos al siguiente caracter
				}
			}
		}
	}
	tilemap.close();																		// Una vez que leimos todo y llenamos el vector del nivel, cerramos el archivo.

	int tileHeight = 256 / 4;																// Alto y ancho de cada tile
	int tileWidht = 256 / 4;

	viewHeight = (winHeight / tileHeight) + 4;												// La altura de la vista va a ser determinada por la ventana que utilizemos y el tamaño de los tiles, mas las dos columnas que necesitamos para swapear
	viewWidth = (winWidth / tileWidht) + 4;													// Lo mismo de lo de arriba, lo que cambia es que es para el ancho.

	xLevel = viewHeight;																	// Me guardo la altura y el ancho para usarlo mas tarde
	yLevel = viewWidth;

	//view = new int[viewWidth, viewHeight];												// Esta es la vista total
	view = new vector<vector<int>*>(viewWidth);												// Inicializo el vector con la cantidad de columnas totales
	for (int i = 0; i < viewWidth; i++)
		view->at(i) = new vector<int>(viewHeight);											// Le agrego las filas al vector

	viewSprite = new vector<vector<Tile*>*>(viewWidth);										// Inicializamos el vector con la cantidad de columnas totales
	for (int i = 0; i < viewWidth; i++)
		viewSprite->at(i) = new vector<Tile*>(viewHeight);									// Le agrego las filas al vector

	UploadSprite();
	LoadView();
}
	
void Tilemap::UploadSprite() {																// Aca cargamos los sprites
	for (int i = 0; i < viewWidth; i++) {													// Recorremos cada posicion de la grilla de tiles
		for (int j = 0; j < viewHeight; j++) {
			viewSprite->at(i)->at(j) = new Tile(render, 1, 1);								// Creo en esa posicion un tile
			viewSprite->at(i)->at(j)->SetMaterial(material);								// Le asigno el material
			viewSprite->at(i)->at(j)->UploadTexture("empty.bmp");							// Le cargo la textura con la que no va a colisionar
			viewSprite->at(i)->at(j)->UploadTexture("pastote.bmp");							// Le cargo la textura con la que si va a colisionar
		}
	}
}

void Tilemap::LoadView() {																	// Cargamos la vista
	int posX = -13;																			// Posicion en x del tilemap
	int posY = 9;																			// Posicion en y del tilemap

	for (int i = 0; i < levelWidth; i++) {
		posX = -13;																			// Posicion de x al comenzar
		for (int j = 0; j < levelHeight; j++) {
			if (i < viewWidth && j < viewHeight) {
				view->at(i)->at(j) = level->at(i)->at(j);									// Le asigno la posicion del nivel a la vista de un tile especifico
				if (view->at(i)->at(j) == 0) 												// Si hay un 0 en esa posicion no deberia colisionar 
					viewSprite->at(i)->at(j)->ChangeTexture(0);								// Cambiamos la textura
				
				if (view->at(i)->at(j) == 1)												// Si hay un 1 en esa posicion deberia colisionar 
					viewSprite->at(i)->at(j)->ChangeTexture(1);								// Cambiamos la textura
				
				posX += 2;																	// Posicion de las columnas (determino el espacio entre ellas)
				viewSprite->at(i)->at(j)->SetPos(posX, posY, 0);							// Estoy asignando una posicion especifica donde se asigna el tile
			}
		}
		posY -= 2;																			// La posicion de las filas (determino el espacio entre ellas)
	}
}

void Tilemap::DrawTilemap() {
	for (int i = 0; i < viewWidth; i++) {													// Recorro el archo de la vista
		for (int j = 0; j < viewHeight; j++) {												// Recorro el largo de la vista
			viewSprite->at(i)->at(j)->Draw();												// Dibujo en cada posicion el valor que haya en la misma
		}
	}
}

void Tilemap::UpdateViewX() {																// Aca calculamos como se debe dibujar al moverse a la derecha en el eje x
	int posX = 13;																			// Posicion en x donde comienza a dibujarse al moverse
	int posY = 9;																			// Posicion en y donde comienza a dibujarse al moverse
																							// Updateamos x
	for (int i = 0; i < viewWidth; i++) {
		for (int j = 1; j < viewHeight; j++) {
			view->at(i)->at(j - 1) = view->at(i)->at(j);									// Updateamos la anchura, la X
		}
	}

	for (int i = 0; i < viewWidth; i++) {	
		int pos = level->at(i)->at(xLevel);
		view->at(i)->at(viewHeight - 1) = pos;												// Updateamos la altura, la Y	
	}
																							// volver a dibujar
	for (int j = 0; j < viewWidth; j++) {												
		if (view->at(j)->at(viewHeight - 1) == 0)											// Si hay un 0 en esa posicion no deberia colisionar 
			viewSprite->at(j)->at(lastPosX)->ChangeTexture(0);								// Cambiamos la textura en ese lugar especifico de la grilla
		
		if (view->at(j)->at(viewHeight - 1) == 1) 											// Si hay un 1 en esa posicion deberia colisionar 
			viewSprite->at(j)->at(lastPosX)->ChangeTexture(1);								// Cambiamos la textura
		
		viewSprite->at(j)->at(lastPosX)->SetPos(posX + render->GetCameraPos().x, posY, 0);	// Estoy asignando una posicion especifica donde se asigna el tile
		posY -= 2;																			// La posicion de las filas (determino el espacio entre ellas)
	}
	if (lastPosX < viewHeight - 1)															// Si la ultima posicion de x es menor que la altura (filas) de la vista - 1(sino se sale de rango),
		lastPosX++;																			// cuenta que fila de la matriz tiene que mandar para atras
	else 
		lastPosX = 0;																		
}

void Tilemap::UpdateTilemap() {
	CurrentCameraPos = render->GetCameraPos();												// Obtengo la posicion de la camara actual
	DeltaCameraPos = CurrentCameraPos - LastCameraPos;										// Resto la pos de la camara actual con la posicion de la camara que era en el frame anterior
	LastCameraPos = CurrentCameraPos;														// Igualo la ultima posicion de la camara a la posicion actual de la camara
																							
	//Updateamos X
	scrollX += DeltaCameraPos.x;															// Le sumamos la posicion de la camara al barrido en x
	if (scrollX >= 2) {																				
		if (xLevel < levelWidth - 1)
			xLevel++;
		UpdateViewX();
		scrollX = 0;
	}
	else {
		if (scrollX <= -2) {
			if (xLevel < 0)
				xLevel--;
			UpdateViewX();
			scrollX = 0;
		}
	}
}

bool Tilemap::CollisionMath(BoundingBox * box, Directions dir) {
	for (int i = 0; i < viewWidth; i++) {
		for (int j = 0; j < viewHeight; j++) {
			if (view->at(i)->at(j) <= 1) {													// Recorremos toda la vista para ver que hay en cada casillero (0 no colisiona, +1 colisiona)
				switch (dir) {
				case Up:
					if ((box->GetY() + box->GetHeight()) >= viewSprite->at(i)->at(j)->GetPos().y)
						return true;
					else 
						return false;
					break;
				case Down:
					if ((box->GetY() - box->GetHeight()) <= (-viewSprite->at(i)->at(j)->GetPos().y))
						return true;
					else
						return false;
					break;
				case Left:
					if ((box->GetX() - box->GetWidth()) <= (viewSprite->at(i)->at(j)->GetPos().x) + box->GetWidth())
						return true;
					else
						return false;
					break;
				case Right:
					if ((box->GetX() - box->GetWidth()) >= (viewSprite->at(i)->at(j)->GetPos().x) + box->GetWidth())
						return true;
					else
						return false;
					break;
				default:
					break;
				}
			}
		}
	}
}

Tilemap::~Tilemap() {																		// Libero memoria
	for (int i = 0; i < viewWidth; i++) {
		delete view->at(i);
		for (int j = 0; j < viewHeight; j++)
			delete viewSprite->at(i)->at(j);
		delete viewSprite->at(i);
	}
	delete view;

	for (int i = 0; i < levelWidth; i++)
		delete level->at(i);
	delete level;
}