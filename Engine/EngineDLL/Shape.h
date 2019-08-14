#pragma once
#include "Entity.h"
#include "Material.h"

class ENGINEDLL_API Shape : public Entity {
protected:
	unsigned int bufferId;
	unsigned int colorBufferId;
	bool shouldDispose;
	bool shouldDisposeColor;
	int vtxCount;
	int vtxColorCount;
	float* vertex;
	float* colorVertex;

	Material * material;
public:
	Shape(Renderer * renderer);
	~Shape() { Dispose(); DisposeColor(); };
	void SetVertices(float* vertex, int count);
	void SetColorVertices(float* vertex, int count);
	virtual void Draw() = 0;
	void DrawMesh(int type);
	void DrawIndexMesh(unsigned int * indices, int idxCount, unsigned int indexBufferId);
	void Dispose();
	void DisposeColor();
	void SetMaterial(Material* material);
};