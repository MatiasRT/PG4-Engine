#pragma once
#include "Shape.h"
#include "Material.h"
#include "Importer.h"
#include "Animation.h"
class ENGINEDLL_API Sprite : public Shape {
protected:
	Header header;
	Animation * animation;
	const char* file;
	unsigned int textureId;
	unsigned int txrBufferId;
	bool shouldDisposeTexture;
	int vtxTextureCount;
	float* textureVertex;
	bool collision;
public:
	Sprite(Renderer* renderer, int col, int rows);//, const char * name);
	~Sprite();
	void LoadBMP(const char * name);
	void DisposeTexture();
	void DrawMesh1(int type);
	void Draw() override;
	void SetTextureVertices(int count);
	void SetMaterial(Material* material);
	void SetAnimation(int iF, int fF, float tF);
	void UpdateAnim(float time);
	void SetCollision() { collision = true; }
	bool GetCollision() { return collision; }
};