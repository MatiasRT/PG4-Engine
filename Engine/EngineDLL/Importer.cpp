#include "Importer.h"
#include "Mesh.h"

Header Importer::LoadBMP(const char * name) {

	// Lectura de información del encabezado del archivo
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	FILE * file;
	fopen_s(&file, name, "rb");
	Header h;

	if (CheckFormat(name, header, file)) {
		// Lectura de los enteros desde el arreglo de bytes
		h.dataPos = *(int*)&(header[0x0A]);
		h.imageSize = *(int*)&(header[0x22]);
		h.width = *(int*)&(header[0x12]);
		h.height = *(int*)&(header[0x16]);
	}

	// Algunos archivos BMP tienen un mal formato, así que adivinamos la información faltante
	if (h.imageSize == 0)    
		h.imageSize = h.width * h.height * 3;	// 3 : un byte por cada componente Rojo (Red), Verde (Green) y Azul(Blue)
	if (h.dataPos == 0)      
		h.dataPos = 54;						// El encabezado del BMP está hecho de ésta manera

	// Se crea un buffer
	h.data = new unsigned char[h.imageSize];

	// Leemos la información del archivo y la ponemos en el buffer
	fread(h.data, 1, h.imageSize, file);

	//Todo está en memoria ahora, así que podemos cerrar el archivo
	fclose(file);

	return h;
}

bool Importer::CheckFormat(const char * name, unsigned char header[], FILE * file) {

	if (!file) {
		printf("Image could not be opened\n"); 
		return 0; 
	}

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}


}

void Importer::LoadMesh(const char * fbxFile, const char * txtFile, GameNode * father, Renderer * renderer, Camera * cam) {
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* pScene = importer.ReadFile(fbxFile, ASSIMP_LOAD_FLAGS);

	if (!pScene) 
		printf("Error parsing '%s': '%s'\n", fbxFile, importer.GetErrorString());
	
	glm::vec3 minimumPoints = glm::vec3(0, 0, 0);																	// Creo un vec donde se guardan los puntos minimos del collider que va a contener el mesh
	glm::vec3 maximusPoints = glm::vec3(0, 0, 0);																	// Creo un vec donde se guardan los puntos maximos del collider que va a contener el mesh

	ProcessNodes(txtFile, father, pScene->mRootNode, pScene, renderer, minimumPoints, maximusPoints, cam);			// Creo y proceso los nodos
	CreateColliderVertices(father, minimumPoints, maximusPoints);													// Creo los vertices de los modelos para obtener el collider
	
}

void Importer::ProcessNodes(const char * txtFile, GameNode* father, aiNode* node, const aiScene* scene, 
	Renderer* renderer, glm::vec3 &minimumPoints, glm::vec3 &maximusPoints, Camera * cam) {

	for (int i = 0; i < (int)node->mNumMeshes; i++) {																// Este loop esta determinado segun la cantidad de meshes que tiene el nodo de la escena

		Mesh * mesh = new Mesh(renderer, txtFile, cam);																// Creo un mesh
		InitMesh(scene->mMeshes[node->mMeshes[i]], mesh, minimumPoints, maximusPoints);								// Genero los vectores del mesh en InitMesh
		GameNode * child = new GameNode(renderer);																	// Creo un nodo
		child->AddComponent((Component*)mesh);																		// Le agrego al nodo el mesh creado arriba como componente
		father->AddChild(child);																					// El nodo creado pasa a ser hijo del nodo padre pasado por parametro
	}

	for (int i = 0; i < (int)node->mNumChildren; i++)																// Este loop esta determinado segun la cantidad de meshes que tiene el nodo de la escena
		ProcessNodes(txtFile, father, node->mChildren[i], scene, renderer, minimumPoints, maximusPoints, cam);		// Hacemos recursividad sobre esta funcion para que se repita esto para todos los posibles meshes que tenga como hijos el modelo exportado
}

void Importer::InitMesh(const aiMesh* paiMesh, Mesh * mesh, glm::vec3 &minimumPoints, glm::vec3 &maximusPoints) {
	MeshData * meshD = mesh->GetMeshData();																			// Obtengo la estructura de meshData e inicializop los vectores
	meshD->vertices = new std::vector<float>();													
	meshD->textures = new std::vector<float>();
	meshD->indices = new std::vector<unsigned int>();

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		// Obtengo el punto minimo y maximo de la diagonal del mesh para generar el collider
		if (pPos->x < minimumPoints.x)																				
			minimumPoints.x = pPos->x;		// Minimo en x																		
		if (pPos->x > maximusPoints.x)
			maximusPoints.x = pPos->x;		// Maximo en x
		if (pPos->y < minimumPoints.y)
			minimumPoints.y = pPos->y;		// Minimo en y
		if (pPos->y > maximusPoints.y)
			maximusPoints.y = pPos->y;		// Maximo en y
		if (pPos->z < minimumPoints.z)
			minimumPoints.z = pPos->z;		// Minimo en z
		if (pPos->z > maximusPoints.z)
			maximusPoints.z = pPos->z;		// Maximo en z

		meshD->vertices->push_back(pPos->x);																		// Cargo el vector de vertices con los vertices del mesh
		meshD->vertices->push_back(pPos->y);
		meshD->vertices->push_back(pPos->z);

		meshD->textures->push_back(pTexCoord->x);																	// Cargo el vector de tecturas con coordenadas de texturas del mesh
		meshD->textures->push_back(pTexCoord->y);

	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		meshD->indices->push_back(Face.mIndices[0]);																// Cargo el vector de indices con los indices del mesh
		meshD->indices->push_back(Face.mIndices[1]);
		meshD->indices->push_back(Face.mIndices[2]);
	}

	mesh->SetMeshData(meshD);																						// Le mando al mesh los vectores del meshData cargados
}

void Importer::CreateColliderVertices(GameNode * father, glm::vec3 minimumPoints, glm::vec3 maximusPoints) {
	const int vertices = 8;
	glm::vec3 colVertices[vertices] = {																				// Guardo en este vector los vertices del collider
		glm::vec3(minimumPoints.x, minimumPoints.y, minimumPoints.z),												
		glm::vec3(maximusPoints.x, maximusPoints.y, maximusPoints.z),												

		glm::vec3(minimumPoints.x, maximusPoints.y, minimumPoints.z),												
		glm::vec3(maximusPoints.x, minimumPoints.y, maximusPoints.z),												
		
		glm::vec3(minimumPoints.x, minimumPoints.y, maximusPoints.z),												
		glm::vec3(maximusPoints.x, maximusPoints.y, minimumPoints.z),												
		
		glm::vec3(minimumPoints.x, maximusPoints.y, maximusPoints.z),												
		glm::vec3(maximusPoints.x, minimumPoints.y, minimumPoints.z)												
	};

	if (father->GetNode(0)->GetComponent(ComponentsType::MeshType) != nullptr)										// Pregunto si tiene mesh antes de asignarlo
		((Mesh*)father->GetNode(0)->GetComponent(ComponentsType::MeshType))->collider->SetVertices(colVertices);	// Le asigo el collider al mesh
}