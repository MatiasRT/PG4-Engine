#include "Camera.h"

/****************************************************************/
/* ESTE CODIGO FUE HECHO EN BASE A ESTA PAGINA					*/
/* http://www.lighthouse3d.com/tutorials/view-frustum-culling/  */
/****************************************************************/

Camera::Camera(Renderer * render) : Component(renderer){
	type: ComponentsType::CamerasType;

	right = glm::vec4(1, 0, 0, 0);		//	x
	up = glm::vec4(0, 1, 0, 0);			//	y
	forward = glm::vec4(0, 0, 1, 0);	//	z

	//pos = glm::vec4(0, 0, 0, 1);
	pos = glm::vec4(400, 65, 17, 1);

	renderer = render;
	ViewMatrix = glm::lookAt(
		(glm::vec3)pos,
		(glm::vec3)(pos + forward),
		(glm::vec3)up
	);

	nearDistance = 0.1f;
	farDistance = 1000.0f;
	ratio = 4.0f / 3.0f;
	angle = glm::radians(45.0f);
	renderer->SetPProjectionMatrix(angle, ratio, nearDistance, farDistance);
	SetCamInternals();
	SetCamDef();
	renderer->SetViewMatrix(ViewMatrix);

	bspPlanes = new vector<glm::vec4>();
	bspPlanesNormals = new vector<glm::vec3>();
}

void Camera::Walk(float xAxis, float zAxis) {
	pos = glm::translate(glm::mat4(1.0f), (glm::vec3)((right * xAxis)+(forward * zAxis))) * pos;

	ViewMatrix = glm::lookAt(
		(glm::vec3)pos,
		(glm::vec3)(pos + forward),
		(glm::vec3)up
	);

	SetCamDef();
	renderer->ModifyCamera(ViewMatrix);

	//cout <<"posx"<< pos.x << endl;
	//cout << "posy" << pos.y << endl;
	//cout << "posz" << pos.z << endl;
}

void Camera::Yaw(float xAxis) {

	forward = glm::rotate(glm::mat4(1.0f), xAxis, glm::vec3(up.x, up.y, up.z)) * forward;
	right = glm::rotate(glm::mat4(1.0f), xAxis, glm::vec3(up.x, up.y, up.z)) * right;
	
	ViewMatrix = glm::lookAt(
		(glm::vec3)pos,															
		(glm::vec3)(pos + forward),
		(glm::vec3)up
	);

	SetCamDef();
	renderer->ModifyCamera(ViewMatrix);
}

void Camera::Pitch(float yAxis) {

	forward = glm::rotate(glm::mat4(1.0f), yAxis, glm::vec3(right.x, right.y, right.z)) * forward;
	up = glm::rotate(glm::mat4(1.0f), yAxis, glm::vec3(right.x, right.y, right.z)) * up;

	ViewMatrix = glm::lookAt(
		(glm::vec3)pos,
		(glm::vec3)(pos + forward),
		(glm::vec3)up
	);

	SetCamDef();
	renderer->ModifyCamera(ViewMatrix);
}

void Camera::Roll(float zAxis) {

	right = glm::rotate(glm::mat4(1.0f), zAxis, glm::vec3(forward.x, forward.y, forward.z)) * right;
	up = glm::rotate(glm::mat4(1.0f), zAxis, glm::vec3(forward.x, forward.y, forward.z)) * up;

	ViewMatrix = glm::lookAt(
		(glm::vec3)pos,
		(glm::vec3)(pos + forward),
		(glm::vec3)up
	);

	SetCamDef();
	renderer->ModifyCamera(ViewMatrix);
}

void Camera::SetCamInternals() {
	// Compute width and height of the near and far plane sections
	tang = glm::tan(angle * 0.5f);
	nearHeight = nearDistance * tang;
	nearWidth = nearHeight * ratio;
}

void Camera::SetCamDef() {
	glm::vec3 rightV = (glm::vec3)right;
	glm::vec3 upV = (glm::vec3)up;

	// Compute the centers of the near and far planes
	glm::vec3 nearCenter = (glm::vec3)pos + (glm::vec3)forward * nearDistance;
	glm::vec3 farCenter = (glm::vec3)pos + (glm::vec3)forward * farDistance;

	// Compute the 4 corners of the frustum on the near plane and far plane
	glm::vec3 leftPlaneVec = (nearCenter - rightV * nearWidth) - (glm::vec3)pos;
	glm::vec3 rightPlaneVec = (nearCenter + rightV * nearWidth) - (glm::vec3)pos;
	glm::vec3 topPlaneVec = (nearCenter + upV * nearHeight) - (glm::vec3)pos;
	glm::vec3 bottomPlaneVec = (nearCenter - upV * nearHeight) - (glm::vec3)pos;
	glm::vec3 normalLeft = glm::normalize(glm::cross(leftPlaneVec, upV));
	glm::vec3 normalRight = -glm::normalize(glm::cross(rightPlaneVec, upV));
	glm::vec3 normalTop = glm::normalize(glm::cross(topPlaneVec, rightV));
	glm::vec3 normalBottom = -glm::normalize(glm::cross(bottomPlaneVec, rightV));

	// Compute the six planes
	plane[Near] = GeneratePlane(-(glm::vec3)forward, nearCenter);
	plane[Far] = GeneratePlane((glm::vec3)forward, farCenter);
	plane[Left] = GeneratePlane(normalLeft, (glm::vec3)pos);
	plane[Right] = GeneratePlane(normalRight, (glm::vec3)pos);
	plane[Top] = GeneratePlane(normalTop, (glm::vec3)pos);
	plane[Bottom] = GeneratePlane(normalBottom, (glm::vec3)pos);
}

glm::vec4 Camera::GeneratePlane(glm::vec3 normal, glm::vec3 point) {			// Genero los planos
	glm::vec4 plane;

	plane.x = normal.x;
	plane.y = normal.y;
	plane.z = normal.z;
	plane.w = -glm::dot(normal, point);

	return plane;
}

int Camera::BoxInFrustum(Collider * collider) {
	bool inside = true;
	bool outside = false;
	int corners = 8;

	for (int i = 0; i < (int)Planes::Count; i++) {								// Este loop se ejecuta por cada plano
		outside = false;

		for (int j = 0; j < corners; j++) {										// Este loop se ejecuta por cada corner
			glm::vec3 vertexPosition = collider->GetVertices(j);				// Me guardo la posicion de un vertice
			glm::vec3 planeNormal = glm::vec3(plane[i]);						// Me guardo la normal del plano

			float dist = glm::dot(planeNormal, vertexPosition) + plane[i].w;	// Obtengo la distancia entre el plano y el vertice para determinar si esta adentro del frustum
			if (dist < 0.0f)
				break;
			if (j == (corners - 1))
				outside = true;
		}
		if (outside) {
			inside = false;
			break;
		}
	}
	if (inside)
		return CameraStates::In;												// Esta adentro del frustum
	else
		return CameraStates::Out;												// Esta afuera del frustum
}

void Camera::AddBSP(Mesh * plane, glm::vec3 nodepos)
{
	if (!plane->IsBSP())
		return;

	bspPlanes->push_back(GeneratePlane(nodepos, plane->GetForwardBSP()));		// Me guardo en el vector de bsp los planos que genero
	bspPlanesNormals->push_back(plane->GetForwardBSP());						// Me guardo en el vector de normales las normales de los planos
}

int Camera::BoxInBSP(Collider * collider)
{
	bool inTheSamePosition = false;
	for (int i = 0; i < bspPlanes->size(); i++) {								// Recorro todos los BSP
		float cameraDistanceToPlane = GetDistanceToPlane(pos, bspPlanes->at(i), bspPlanesNormals->at(i));	// Obtengo la distancia de la camara al plano x
		float cameraDistanceSign = glm::sign(cameraDistanceToPlane);										// Obtengo el signo para saber de que lado esta la camara
		for (int j = 0; j < 8; j++)												// Recorro en 8 (cant de vertices en un cubo)
		{
			glm::vec3 vertexPosition = collider->GetVertices(j);				// Obtengo los vertices
			float vertexDistanceToPlane = GetDistanceToPlane(vertexPosition, bspPlanes->at(i), bspPlanesNormals->at(i));	// Obtengo la distancia de cada vertice del collider x al plano x.
			float vertexDistanceSign = glm::sign(vertexDistanceToPlane);													// Obtengo el signo para saber de que lado esta.

			if (vertexDistanceSign == cameraDistanceSign)
				break;
			if (j == 8 -1)
				inTheSamePosition = true;
		}
	}
	if (!inTheSamePosition)
		return CameraStates::In;
	else
		return CameraStates::Out;
}

float Camera::GetDistanceToPlane(glm::vec3 point, glm::vec4 plane, glm::vec3 planeNormal)
{
	float distance = 0.0f;

	distance = glm::dot(planeNormal, point) + plane.w;

	return distance;
}