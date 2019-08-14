#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Exports.h"
#include "Window.h"
#include "Definitions.h"
#include <vector>
#include <iostream>
using namespace std;

class ENGINEDLL_API Renderer {
	unsigned int VertexArrayID;
	Window* window;
	glm::mat4 WorldMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 wvp;

	glm::mat4 OthoProMatrix;
	glm::mat4 PerspectiveProMatrix;

	glm::vec3 camera;
	glm::vec3 eye;
	glm::vec3 upAxis;

public:
	Renderer() { };
	~Renderer() { };
	bool Start(Window* windowPtr);
	bool Stop();
	void ClearScreen();
	void ClearColor(float r, float g, float b, float a);
	void SwapBuffers();
	unsigned int GenBuffer(float* buffer, int size);
	unsigned int GenColorBuffer(float* buffer, int size);
	unsigned int GenElementBuffer(unsigned int * indices, int size);
	unsigned int UploadData(float width, float height, const void* data);
	void DrawBuffer(int size, int type);
	void DrawElementBuffer(int size, unsigned int indexBuffer);
	void DestroyBuffer(unsigned int buffer);
	void UpdateWVP();
	void LoadIdentityMatrix();
	void SetModelMatrix(glm::mat4 mat);
	void MultiplyModelMatrix(glm::mat4 mat);
	glm::mat4& GetWVP() { return wvp; }
	void BeginDraw(unsigned int attribute);
	void BindBuffer(unsigned int vtxbuffer, unsigned int attribute);
	void BindColorBuffer(unsigned int clrbuffer, unsigned int attribute);
	void BindTextureBuffer(unsigned int txrbuffer, unsigned int attribute);
	void BindTexture(unsigned int textureID, unsigned int txrbufferID);
	void EndDraw(unsigned int attribute);
	void UpdateTexture(unsigned int textureID);
	void ModifyCamera(glm::mat4 mat);
	glm::vec3 GetCameraPos() { return camera; };

	void SetOProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetPProjectionMatrix(float fovY, float aspect, float zNear, float zFar);
	void ChangeProjectionMatrix(CameraType camera);
	void SetViewMatrix(glm::vec3 eye, glm::vec3 camera, glm::vec3 upAxis);

	void SetWorldMatrix(glm::mat4 worldMatrix);
	void SetViewMatrix(glm::mat4 viewMatrix);
	void SetProjectionMatrix(glm::mat4 projectionMatrix);

	glm::mat4 GetViewMatrix() { return ViewMatrix; };
	glm::mat4 GetWorldMatrix() { return WorldMatrix; };
	glm::mat4 GetProjectionMatrix() { return ProjectionMatrix; };

	void BindBuffer(unsigned int atribID, unsigned int vtxBuffer, unsigned int size);
};