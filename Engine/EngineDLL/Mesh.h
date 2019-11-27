#ifndef MESH_H
#define MESH_H
#include "Importer.h"
#include "Component.h"
#include "GameNode.h"
#include "Collider.h"
#include "Camera.h"

struct MeshInfo {
	unsigned int bufferId;
	unsigned int textureBufferId;
	unsigned int indexBufferId;
	unsigned int textureId;
	bool shouldDisposeVertices;
	bool shouldDisposeIndices;
	bool shouldDisposeTexture;
	unsigned int vtxCount;
	unsigned int textureCount;
	unsigned int idxCount;
	float * vertex;
	float * textureVertex;
	unsigned int * indices;
	Material * material;
	unsigned int programId;
};

class ENGINEDLL_API Mesh : public Component {
	Header header;

	unsigned int textureId;
	const char* textureFile;

	MeshInfo* meshInfo;
	MeshData* meshData;

	Camera * camera;

	void DisposeVertices();
	void DisposeIndices();
	void DisposeTexture();

	/* BSP */
	bool isBSP = false;
	glm::vec3 bspVecForward;

public:
	Mesh(Renderer* renderer, const char* txtFile, Camera * cam);
	~Mesh();

	void SetMeshData(MeshData * meshD);
	void Draw() override;
	
	MeshData* GetMeshData() { return meshData; };
	Collider* collider;

	/* BSP */
	void UpdateData(glm::vec3 min, glm::vec3 max);
	void SetBSP(bool bsp, GameNode * node);
	bool IsBSP() { return isBSP; }
	glm::vec3 GetForwardBSP() { return bspVecForward; }
};
#endif