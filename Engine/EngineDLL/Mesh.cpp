#include "Mesh.h"

Mesh::Mesh(Renderer * renderer, const char * txtFile, Camera * cam) : Component(renderer) {
	type = ComponentsType::MeshType;
	camera = cam;
	collider = new Collider(renderer);

	textureFile = txtFile;
	meshInfo = new MeshInfo;
	meshData = new MeshData;

	meshInfo->shouldDisposeVertices = false;
	meshInfo->shouldDisposeIndices = false;
	meshInfo->shouldDisposeTexture = false;

	meshInfo->bufferId = -1;
	meshInfo->textureBufferId = -1;
	meshInfo->indexBufferId = -1;
}

Mesh::~Mesh() {
	delete[] meshInfo->vertex;
	delete[] meshInfo->textureVertex;
	delete[] meshInfo->indices;
	delete meshInfo->material;
	delete meshInfo;
	delete meshData;
	delete collider;
}

void Mesh::SetMeshData(MeshData * meshD) {
	meshData = meshD;
	meshInfo->vtxCount = meshData->vertices->size();														// Guardo la cantidad de vertices que tiene el meshData luego de ser cargado en InitMesh de importer
	meshInfo->idxCount = meshData->indices->size();															// Guardo la cantidad de indices que tiene el meshData luego de ser cargado en InitMesh de importer
	meshInfo->textureCount = meshData->textures->size();													// Guardo la cantidad de texuras que tiene el meshData luego de ser cargado en InitMesh de importer

	meshInfo->vertex = new float[meshInfo->vtxCount];
	for (int i = 0; i < meshInfo->vtxCount; i++)
		meshInfo->vertex[i] = meshData->vertices->at(i);													// Cargo el vector de vertices de meshInfo con los vertices de meshData

	DisposeVertices();

	meshInfo->shouldDisposeVertices = true;
	meshInfo->bufferId = renderer->GenBuffer(meshInfo->vertex, sizeof(float)* meshInfo->vtxCount);			// Genero el buffer para los vertices


	meshInfo->textureVertex = new float[meshInfo->textureCount];
	for (int k = 0; k < meshInfo->textureCount; k++) 
		meshInfo->textureVertex[k] = meshData->textures->at(k);												// Cargo el vector de texturas de meshInfo con las texturas de meshData

	DisposeTexture();

	meshInfo->shouldDisposeTexture = true;
	meshInfo->textureBufferId = renderer->GenBuffer(meshInfo->textureVertex, sizeof(float)* meshInfo->textureCount); // Genero el buffer para las texturas

	meshInfo->indices = new unsigned int[meshInfo->idxCount];
	for (int j = 0; j < meshInfo->idxCount; j++) 
		meshInfo->indices[j] = meshData->indices->at(j);													// Cargo el vector de indices de meshInfo con los indices de meshData
	
	DisposeIndices();

	meshInfo->shouldDisposeIndices = true;
	meshInfo->indexBufferId = renderer->GenElementBuffer(meshInfo->indices, sizeof(int)* meshInfo->idxCount);		// Genero el buffer para los indices

	meshInfo->material = new Material();																	// Creo el material
	meshInfo->programId = meshInfo->material->LoadShaders("VertexTexture.glsl", "FragmentTexture.glsl");	// Le cargo los shaders

	header = Importer::LoadBMP(textureFile);																// Cargo la textura en formato bmp
	meshInfo->textureId = renderer->UploadData(header.width, header.height, header.data);
	meshInfo->material->BindTexture();
}

void Mesh::Draw() {
	if (camera->BoxInFrustum(collider) == CameraStates::In) {
		cout << "Mesh Inside Frustum" << endl;
		if (meshInfo->material != NULL) {
			meshInfo->material->Bind();
			meshInfo->material->SetMatrixProperty(renderer->GetWVP());
		}
		renderer->BindTexture(meshInfo->textureId, meshInfo->textureBufferId);
		renderer->BeginDraw(0);																				// Le decimos al renderer que comience a dibujar
		renderer->BeginDraw(1);
		renderer->BindBuffer(meshInfo->bufferId, 0);														// Unimos el buffer con el buffer binding point
		renderer->BindBuffer(1, meshInfo->textureBufferId, 2);
		renderer->DrawElementBuffer(meshInfo->idxCount, meshInfo->indexBufferId);							// El renderer dibuja el triangulo
		renderer->EndDraw(0);																				// Deja de dibujar
		renderer->EndDraw(1);
	}
	else
		cout << "Mesh Outside Frustum" << endl;
}

void Mesh::DisposeVertices() {
	if (meshInfo->shouldDisposeVertices) {
		renderer->DestroyBuffer(meshInfo->bufferId);
		meshInfo->shouldDisposeVertices = false;
	}
}

void Mesh::DisposeIndices() {
	if (meshInfo->shouldDisposeIndices) {
		renderer->DestroyBuffer(meshInfo->indexBufferId);
		meshInfo->shouldDisposeIndices = false;
	}
}

void Mesh::DisposeTexture() {
	if (meshInfo->shouldDisposeTexture) {
		renderer->DestroyBuffer(meshInfo->textureBufferId);
		meshInfo->shouldDisposeTexture = false;
	}
}