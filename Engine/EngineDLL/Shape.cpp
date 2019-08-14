#include "Shape.h"

Shape::Shape(Renderer * renderer):Entity(renderer) {

	shouldDispose = false;
	shouldDisposeColor = false;
	material = NULL;
	vertex = NULL;
	colorVertex = NULL;
	bufferId = -1;
	colorBufferId = -1;
}

void Shape::SetVertices(float* vertex, int count) {
	Dispose();

	vtxCount = count;
	shouldDispose = true;

	bufferId = renderer->GenBuffer(vertex, sizeof(float) * count * 3);
}

void Shape::SetColorVertices(float* vertex, int count) {
	DisposeColor();

	vtxColorCount = count;
	shouldDisposeColor = true;

	colorBufferId = renderer->GenColorBuffer(vertex, sizeof(float) * count * 3);
}

void Shape::DrawMesh(int type) {
	renderer->LoadIdentityMatrix();
	renderer->SetModelMatrix(worldMatrix);

	if (material != NULL) {
		material->Bind();
		material->SetMatrixProperty(renderer->GetWVP());
	}
	renderer->BeginDraw(0);													// Le decimos al renderer que comience a dibujar
	renderer->BeginDraw(1);
	renderer->BindBuffer(bufferId, 0);										// Unimos el buffer con el buffer binding point
	renderer->BindColorBuffer(colorBufferId, 1);
	renderer->DrawBuffer(vtxCount, type);									// El renderer dibuja el triangulo
	renderer->EndDraw(0);													// Deja de dibujar
	renderer->EndDraw(1);
}

void Shape::DrawIndexMesh(unsigned int * indices, int idxCount, unsigned int indexBufferId) {
	renderer->LoadIdentityMatrix();
	renderer->SetModelMatrix(worldMatrix);

	if (material != NULL) {
		material->Bind();
		material->SetMatrixProperty(renderer->GetWVP());
	}
	renderer->BeginDraw(0);													// Le decimos al renderer que comience a dibujar
	renderer->BeginDraw(1);
	renderer->BindBuffer(bufferId, 0);								// Unimos el buffer con el buffer binding point
	renderer->BindColorBuffer(colorBufferId, 1);
	renderer->DrawElementBuffer(idxCount, indexBufferId);			// El renderer dibuja el triangulo
	renderer->EndDraw(0);													// Deja de dibujar
	renderer->EndDraw(1);
}

void Shape::Dispose() {
	if (shouldDispose) {
		renderer->DestroyBuffer(bufferId);
		shouldDispose = false;
	}
}

void Shape::DisposeColor() {
	if (shouldDisposeColor) {
		renderer->DestroyBuffer(colorBufferId);
		shouldDisposeColor = false;
	}
}

void Shape::SetMaterial(Material* material) {
	this->material = material;
}
