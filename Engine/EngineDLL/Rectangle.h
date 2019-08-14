#pragma once
#include "Shape.h"
#include "Material.h"
class ENGINEDLL_API Rectangle: public Shape {
public:
	Rectangle(Renderer * renderer);
	~Rectangle();
	void Draw() override;
};