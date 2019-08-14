#pragma once
#include "Definitions.h"
#include "Exports.h"
#include "Renderer.h"
#include "BoundingBox.h"
#include "BoundingCircle.h"
#include <iostream>
using namespace std;
class ENGINEDLL_API Entity {
protected:
	Renderer * renderer;

	glm::vec3 v3pos;
	glm::vec3 v3scale;
	glm::vec3 v3rot;

	glm::mat4 worldMatrix;
	glm::mat4 translateMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 rotMatrix;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;

	BoundingBox* box;
	BoundingCircle* circle;
public:
	virtual void Draw() = 0;
	Entity(Renderer* rendererPtr);
	void SetPos(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotX(float x);
	void SetRotY(float y);
	void SetRotZ(float z);
	void Rotate(float x, float y, float z);
	void TranslationBox(float x, float y, float z);
	void TranslationCircle(float x, float y, float z);
	void SetBoundingBox(float w, float h, float mass, bool setStatic, bool setTrigger);
	void SetBoundingCircle(float r, float mass, bool setStatic, bool setTrigger);
	glm::vec3 GetPos() { return v3pos; };
	glm::vec3 GetScale() { return v3scale; };
	glm::vec3 GetRot() { return v3rot; };
	BoundingBox* GetBoundingBox() { return box; };
	BoundingCircle* GetBoundingCircle() { return circle; };
	void UpdateWorldMatrix();
};