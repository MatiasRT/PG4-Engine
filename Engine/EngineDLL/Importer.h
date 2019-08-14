#ifndef IMPORTER_H
#define IMPORTER_H
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices)
#include "Exports.h"
#include "Definitions.h"
#include "GameNode.h"
#include "Camera.h"
#include <fstream>
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assert.h"
#include "assimp/postprocess.h"

class Mesh;

struct Header {
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char * data;
};

static class ENGINEDLL_API Importer {
	
	static void InitMesh(const aiMesh* paiMesh, Mesh* mesh, glm::vec3 &minimumPoints, glm::vec3 &maximusPoints);
	static void ProcessNodes(const char* txtFile, GameNode* father, 
		aiNode* node, const aiScene* scene, Renderer* renderer,
		glm::vec3 &minimumPoints, glm::vec3 &maximusPoints, Camera * cam);
	static void CreateColliderVertices(GameNode* father, glm::vec3 minimumPoints, glm::vec3 maximusPoints);

public:

	static Header LoadBMP(const char * name);
	static bool CheckFormat(const char * name, unsigned char header[], FILE * file);

	static void LoadMesh(const char * fbxFile, const char * txtFile, GameNode * father, Renderer * renderer, Camera * cam);
};
#endif