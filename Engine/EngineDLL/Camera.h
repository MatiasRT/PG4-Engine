#pragma once
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Exports.h"
#include "Entity.h"
#include "Renderer.h"
#include "Component.h"
#include "Collider.h"
#include "Mesh.h"

/****************************************************************/
/* ESTE CODIGO FUE HECHO EN BASE A ESTA PAGINA					*/
/* http://www.lighthouse3d.com/tutorials/view-frustum-culling/  */
/****************************************************************/

using namespace std;
class ENGINEDLL_API Camera : public Component {
	
	enum Planes {
		Top = 0,
		Bottom,
		Left,
		Right,
		Far,
		Near,
		Count
	};

	Renderer* renderer;
	glm::mat4 ViewMatrix;

	CameraType camType;

	glm::vec4 forward;
	glm::vec4 right;
	glm::vec4 up;
	glm::vec4 pos;

	/* BSP */
	vector<glm::vec4> * bspPlanes;
	vector<glm::vec3> * bspPlanesNormals;

	glm::vec4 plane[Count];

	float nearDistance;
	float farDistance;
	float ratio;
	float angle;
	float tang;

	float nearWidth;
	float nearHeight;

public:
	Camera(Renderer * render);
	~Camera() {};
	
	void Walk(float xAxis, float zAxis);
	void Yaw(float xAxis);
	void Pitch(float yAxis);
	void Roll(float zAxis);

	void SetCamInternals();
	void SetCamDef();
	glm::vec4 GeneratePlane(glm::vec3 normal, glm::vec3 point);
	int BoxInFrustum(Collider * BundingCube);

	/* BSP */
	void AddBSP(Mesh * plane, glm::vec3 nodePos);
	int BoxInBSP(Collider * collider);

	float GetDistanceToPlane(glm::vec3 point, glm::vec4 plane, glm::vec3 planeNormal);
};