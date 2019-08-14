#include "Sprite.h"

Sprite::Sprite(Renderer* renderer, int col, int rows) : Shape(renderer) {
	collision = false;
	vertex = new float[12]{													
		-1.0f, -1.0f, 0.0f,													
		-1.0f, 1.0f, 0.0f,													
		1.0f, -1.0f, 0.0f,													
		1.0f, 1.0f, 0.0f										
	};

	/*textureVertex = new float[8]{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};*/

	animation = new Animation(col, rows);
	textureVertex = animation->UpdateAnimation(0);

	SetVertices(vertex, 4);
	SetTextureVertices(4);
}

void Sprite::LoadBMP(const char * name) {
	header = Importer::LoadBMP(name);
	textureId = renderer->UploadData(header.width, header.height, header.data);
	material->BindTexture();
}

void Sprite::SetTextureVertices(int count) {
	DisposeTexture();

	vtxTextureCount = count;
	shouldDisposeTexture = true;

	txrBufferId = renderer->GenBuffer(textureVertex, sizeof(float) * count * 2);
}

void Sprite::DrawMesh1(int type) {
	renderer->LoadIdentityMatrix();
	renderer->SetModelMatrix(worldMatrix);

	if (material != NULL) {
		material->Bind();
		material->SetMatrixProperty(renderer->GetWVP());
	}
	renderer->BindTexture(textureId, txrBufferId);
	renderer->BeginDraw(0);
	renderer->BeginDraw(1);																	// Le decimos al renderer que comience a dibujar
	renderer->BindBuffer(bufferId, 0);
	renderer->BindTextureBuffer(txrBufferId, 1);											// Unimos el buffer con el buffer binding point
	renderer->DrawBuffer(vtxCount, type);													// El renderer dibuja el triangulo
	renderer->EndDraw(0);
	renderer->EndDraw(1);																	// Deja de dibujar
}

void Sprite::DisposeTexture() {
	if (shouldDisposeTexture) {
		renderer->DestroyBuffer(txrBufferId);
		shouldDisposeTexture = false;
	}
}

void Sprite::Draw() {
	DrawMesh1(GL_TRIANGLE_STRIP);
}

Sprite::~Sprite() {
	delete[] vertex;
	delete[] textureVertex;
}

void Sprite::SetMaterial(Material* material) {
	this->material = material;
}

void Sprite::SetAnimation(int iF, int fF, float tF) {
	animation->SetAnimation(iF, fF, tF);
}

void Sprite::UpdateAnim(float time) {
	textureVertex = animation->UpdateAnimation(time);
	SetTextureVertices(4);;
}