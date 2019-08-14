#include "Renderer.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>

bool Renderer::Start(Window* windowPtr) {
	if (windowPtr != NULL) {
		window = windowPtr;
		glfwMakeContextCurrent((GLFWwindow*)window->GetWindow());						// Le dice cual es la ventana que va a usar
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//GLuint VertexArrayID;
		if (glewInit() != GLEW_OK) {
			cout << "Fallo al inicializar GLEW\n" << endl;
			return -1;
		}
		glGenVertexArrays(1, &VertexArrayID);											// Creamos el Vertex Array Object. Esto debe hacerse antes de cualquier llamada a una funcion OpenGL
		glBindVertexArray(VertexArrayID);												// Creamos el Vertex Array Object

		glEnable(GL_DEPTH_TEST);														// Habilitamos el depth buffer
		glDepthFunc(GL_LESS);

		ProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);		//Usamos una projeccion ortogonal

		OthoProMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);			// OrthoView predeterminada
		PerspectiveProMatrix = glm::perspective(10.0f, 16.0f / 9.0f, 0.1f, 100.0f);		// PersView predeterminada

		ViewMatrix = glm::lookAt(
			glm::vec3(0, 0, 3),															// Camara en 0,0,3 en el World space
			glm::vec3(0, 0, 0),															// Mira hacia el origen
			glm::vec3(0, 1, 0)															// La cabeza esta hacia arriba (0, -1, 0 va a mirar cabeza abajo, dado vuelta) 
		);

		WorldMatrix = glm::mat4(1.0f);													// Seteamos la matriz de mundo
		
		UpdateWVP();

		camera = glm::vec3(0, 0, 0);
		eye = glm::vec3(0, 0, 3);
		upAxis = glm::vec3(0, 1, 0);

		cout << "Renderer::Start()" << endl;
		return true;
	}
	return false;
}

bool Renderer::Stop() {
	cout << "Renderer::Stop()" << endl;
	return false;
}

void Renderer::ClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void Renderer::SwapBuffers() {
	glfwSwapBuffers((GLFWwindow*)window->GetWindow());
}

unsigned int Renderer::GenBuffer(float* buffer, int size) {								// Aca creamos el buffer. Esto solo debe hacerse una vez

	unsigned int vertexbuffer;															// Identificamos el vertex buffer
	glGenBuffers(1, &vertexbuffer);														// Generamos un buffer, y ponemos el resultado en el vertexbuffer que acabamos de crear
																						 
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);										// Los siguientes comandos le darán características especiales al 'vertexbuffer'
																					
	glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);						// Le damos a OpenGL nuestros vertices
	return vertexbuffer;
}

unsigned int Renderer::GenColorBuffer(float* buffer, int size) {

	unsigned int colorbuffer;
	glGenBuffers(1, &colorbuffer);

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);

	glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
	return colorbuffer;
}

unsigned int Renderer::GenElementBuffer(unsigned int * indices, int size) {
	unsigned int elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	return elementbuffer;
}

/* REVISAR ESTO TAMBIEN QUE SE REPITE EN LOS GEN */

unsigned int Renderer::UploadData(float width, float height, const void* data) {

	unsigned int texturebuffer;
	glGenTextures(1, &texturebuffer);

	glBindTexture(GL_TEXTURE_2D, texturebuffer);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return texturebuffer;
}

void Renderer::DestroyBuffer(unsigned int buffer) {
	glDeleteBuffers(1, &buffer);														// Recibe el buffer que hay que eliminar, y lo hacemos con una funcion de OpenGL
}

void Renderer::LoadIdentityMatrix() {
	WorldMatrix = glm::mat4(1.0f);
}

void Renderer::SetModelMatrix(glm::mat4 mat) {
	WorldMatrix = mat;
	UpdateWVP();
}

void Renderer::MultiplyModelMatrix(glm::mat4 mat) {
	WorldMatrix *= mat;
	UpdateWVP();
}

void Renderer::UpdateWVP() {
	wvp = ProjectionMatrix * ViewMatrix * WorldMatrix;
}

void Renderer::BeginDraw(unsigned int attribute){
	glEnableVertexAttribArray(attribute);												// Le decimos que puede comenzar a dibujar
}

void Renderer::BindBuffer(unsigned int vtxbuffer, unsigned int attribute){				// Binds a buffer object to the specified buffer binding point
	glBindBuffer(GL_ARRAY_BUFFER, vtxbuffer);
	glVertexAttribPointer(
		attribute,																		// atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
		3,																				// tamaño
		GL_FLOAT,																		// tipo
		GL_FALSE,																		// normalizado?
		0,																				// Paso
		(void*)0																		// desfase del buffer
	);
}

void Renderer::BindColorBuffer(unsigned int clrbuffer, unsigned int attribute) {
	glBindBuffer(GL_ARRAY_BUFFER, clrbuffer);
	glVertexAttribPointer(
		attribute,														
		3,																
		GL_FLOAT,														
		GL_FALSE,														
		0,																
		(void*)0
	);
}

void Renderer::BindTextureBuffer(unsigned int txrbuffer, unsigned int attribute) {
	glBindBuffer(GL_ARRAY_BUFFER, txrbuffer);
	glVertexAttribPointer(
		attribute,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
}
/* REVISAR LOS BIND PORQUE SE PUEDEN UNIR Y CAMBIAR SOLO EL TAMAÑO POR PARAMETRO */

void Renderer::BindTexture(unsigned int textureID, unsigned int txrbufferID) {
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(txrbufferID, 0);
}

void Renderer::DrawBuffer(int size, int type) {											// Dibujamos en el buffer
	glDrawArrays(type, 0, size);														// Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo
}

void Renderer::DrawElementBuffer(int size, unsigned int indexBuffer) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(
		GL_TRIANGLES,		// mode
		size,				// count
		GL_UNSIGNED_INT,	// type
		(void*)0			// element array buffer offset
	);
}

void Renderer::EndDraw(unsigned int attribute){
	glDisableVertexAttribArray(attribute);												// Le decimos que finalice de dibujar en el buffer
}

void Renderer::UpdateTexture(unsigned int textureID) {
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Renderer::ModifyCamera(glm::mat4 mat) {
	ViewMatrix = mat;
	UpdateWVP();
}

void Renderer::SetOProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar) {
	ProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
	OthoProMatrix = ProjectionMatrix;
	UpdateWVP();
}

void Renderer::SetPProjectionMatrix(float fovY, float aspect, float zNear, float zFar) {
	ProjectionMatrix = glm::perspective(fovY, aspect, zNear, zFar);
	PerspectiveProMatrix = ProjectionMatrix;
	UpdateWVP();
}

void Renderer::ChangeProjectionMatrix(CameraType camera) {
	switch (camera) {
		case ortho:
			ProjectionMatrix = OthoProMatrix;
			break;
		case perspective:
			ProjectionMatrix = PerspectiveProMatrix;
			break;
		default:
			ProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
			break;
	}
	UpdateWVP();
}

void Renderer::SetViewMatrix(glm::vec3 eye, glm::vec3 camera, glm::vec3 upAxis) {
	ViewMatrix = glm::lookAt(
		eye,															// Camara en 0,0,3 en el World space
		camera,															// Mira hacia el origen
		upAxis															// La cabeza esta hacia arriba (0, -1, 0 va a mirar cabeza abajo, dado vuelta) 
	);
	WorldMatrix = glm::mat4(1.0f);
	UpdateWVP();
}

void Renderer::SetWorldMatrix(glm::mat4 worldMatrix) {
	WorldMatrix = worldMatrix;
	UpdateWVP();
}

void Renderer::SetViewMatrix(glm::mat4 viewMatrix) {
	ViewMatrix = viewMatrix;
	UpdateWVP();
}

void Renderer::SetProjectionMatrix(glm::mat4 projectionMatrix) {
	ProjectionMatrix = projectionMatrix;
	UpdateWVP();
}

void Renderer::BindBuffer(unsigned int atribID, unsigned int vtxBuffer, unsigned int size) {				// Binds a buffer object to the specified buffer binding point
	glBindBuffer(GL_ARRAY_BUFFER, vtxBuffer);
	glVertexAttribPointer(
		atribID,            // debe corresponder en el shader.
		size,               // tamaño
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		0,                  // Paso
		(void*)0            // desfase del buffer
	);
}