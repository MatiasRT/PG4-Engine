#include "BoundingBox.h"

BoundingBox::BoundingBox(glm::vec2 p, float w, float h, float m, bool setStatic, bool setTrigger/*, Shape* shape*/) {
	pos = p;
	width = w;
	height = h;
	mass = m;
	isStatic = setStatic;
	isTrigger = setTrigger;
	collision = false;
	//boxShape = shape;
}