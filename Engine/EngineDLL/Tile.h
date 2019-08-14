#pragma once
#include "Exports.h"
#include "Sprite.h"
#include <vector>

class ENGINEDLL_API Tile : public Sprite {
	Header header;
	vector<int>* textures;
public:
	Tile(Renderer * rend, int col, int row);
	~Tile();
	void UploadTexture(const char * file);
	void ChangeTexture(int i);
};