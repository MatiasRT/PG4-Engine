#pragma once
#include "Exports.h"
#include <list>
#include <vector>
using namespace std;

class ENGINEDLL_API Spritesheet {

	//list<float*> * uvList; //vertices de uv
	vector<float*> * uvVector;
	float* uvPositions;

public:
	Spritesheet(int tW, int tH, int col, int rows);
	~Spritesheet() { delete uvVector; };
	int GetSize() { return uvVector->size(); };
	float * SetSpritesheet(int frame);
};