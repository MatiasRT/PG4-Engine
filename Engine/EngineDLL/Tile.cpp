#include "Tile.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>

Tile::Tile(Renderer * rend, int col, int row):Sprite(rend, col, row) {
	textures = new vector<int>();
}

Tile::~Tile() {
	delete textures;
}

void Tile::UploadTexture(const char * file) {
	header = Importer::LoadBMP(file);
	textureId = renderer->UploadData(header.width, header.height, header.data);
	material->BindTexture();
	textures->push_back(textureId);
}

void Tile::ChangeTexture(int i) {
	textureId = textures->at(i);
	renderer->UpdateTexture(textures->at(i));
	material->BindTexture();
}