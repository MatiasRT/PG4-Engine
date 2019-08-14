#pragma once
#include "Exports.h"
#include "Definitions.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
class ENGINEDLL_API BoundingCircle {
	//Layers layer;
	//Shape* boxShape;
	glm::vec2 pos;
	float radius;
	float mass;
	bool isStatic;
	bool isTrigger;
	bool collision;

public:
	BoundingCircle(glm::vec2 pos, float radius, float mass, bool setStatic, bool setTrigger);
	~BoundingCircle() { };
	bool IsStatic() { return isStatic; };
	bool IsTrigger() { return isTrigger; };
	void SetPos(float x, float y) { pos.x = x; pos.y = y; };
	void SetCollision(bool setCollision) { collision = setCollision; };
	float GetX() { return pos.x; };
	float GetY() { return pos.y; };
	float GetRadius() { return radius; };
	float GetMass() { return mass; };
	glm::vec2 GetPos() { return pos; };
	bool GetCollision() { return collision; };
};