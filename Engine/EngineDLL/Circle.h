#pragma once
#include "Shape.h"
#define PI 3.14159265359f
class ENGINEDLL_API Circle : public Shape {
	float radius;
	float angle;
	float degrees;
	int triangleCount;
public:
	Circle(Renderer * renderer, float radio, int count);
	~Circle();
	void Draw() override;
};