#include "BoundingCircle.h"

BoundingCircle::BoundingCircle(glm::vec2 p, float r, float m, bool setStatic, bool setTrigger) {
	pos = p;
	radius = r;
	mass = m;
	isStatic = setStatic;
	isTrigger = setTrigger;
}