#pragma once
#include "Exports.h"
#include "Renderer.h"
#include "Material.h"
#include "BoundingBox.h"
#include "Tile.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

class ENGINEDLL_API Tilemap {
	int viewWidth;
	int viewHeight;
	int levelWidth;
	int levelHeight;
	int xLevel;
	int yLevel;
	float scrollX;
	float scrollY;
	int lastPosX;
	int lastPosY;

	Material * material;
	Renderer * render;

	glm::vec3 LastCameraPos;
	glm::vec3 CurrentCameraPos;
	glm::vec3 DeltaCameraPos;

	vector<vector<Tile*>*>* viewSprite;
	vector<vector<int>*>* level;
	vector<vector<int>*>* view;
	//vector<int>* textures;

	void UploadSprite();
	void LoadView();
	void UpdateViewX();
	void UpdateViewY();

public:
	Tilemap(const char* filepath, int winWidth, int winHeight, Material * mat, Renderer * rend);
	~Tilemap();
	void DrawTilemap();
	void UpdateTilemap();
	bool CollisionMath(BoundingBox * box, Directions dir);
};