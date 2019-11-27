#include "Entity.h"

Entity::Entity(Renderer* rendererPTR) {
	box = NULL;
	circle = NULL;
	renderer = rendererPTR;
	worldMatrix = glm::mat4(1.0f);															// Identidad
	translateMatrix = glm::mat4(1.0f);														// Translacion
	scaleMatrix = glm::mat4(1.0f);															// Escala
	rotMatrix = glm::mat4(1.0f);															// Rotacion
	rotationX = glm::mat4(1.0f);															// Rotacion en X
	rotationY = glm::mat4(1.0f);															// Rotacion en Y
	rotationZ = glm::mat4(1.0f);															// Rotacion en Z
	v3pos[0] = v3pos[1] = v3pos[2] = 0.0f;													// Inicializacion de la posicion
	v3rot[0] = v3rot[1] = v3rot[2] = 0.0f;													// Inicializacion de la rotacion
	v3scale[0] = v3scale[1] = v3scale[2] = 1.0f;											// Inicializacion de la escala
}

void Entity::SetPos(float x, float y, float z) {
	if (box == NULL || circle == NULL) {													// No hay caja de colision
		v3pos[0] = x;
		v3pos[1] = y;
		v3pos[2] = z;
	}
	else {
		if (box != NULL && !box->GetCollision()) {											// Hay caja/circunferencia de colision, verifica el resultado del getter de colision de la caja/circunferencia
			v3pos[0] = x;
			v3pos[1] = y;
			v3pos[2] = z;
			box->SetPos(v3pos[0], v3pos[1]);
			box->SetCollision(false);	
		}
		if (circle != NULL && !circle->GetCollision()) {
			v3pos[0] = x;
			v3pos[1] = y;
			v3pos[2] = z;
			circle->SetPos(v3pos[0], v3pos[1]);
			circle->SetCollision(false);
		}
	}

	translateMatrix = glm::translate(glm::mat4(1.0f), v3pos);
	UpdateWorldMatrix();
}

void Entity::SetScale(float x, float y, float z) {
	v3scale[0] = x;
	v3scale[1] = y;
	v3scale[2] = z;

	scaleMatrix = glm::scale(glm::mat4(1.0f), v3scale);
	UpdateWorldMatrix();
}

void Entity::SetRotX(float x) {
	v3rot[0] = x;
	
	glm::vec3 axis;
	axis[2] = axis[1] = 0.0f;
	axis[0] = 1.0f;

	rotationX = glm::rotate(glm::mat4(1.0f), x, axis);
	
	UpdateWorldMatrix();
}

void Entity::SetRotY(float y) {
	v3rot[1] = y;

	glm::vec3 axis;
	axis[2] = axis[0] = 0.0f;
	axis[1] = 1.0f;

	rotationY = glm::rotate(glm::mat4(1.0f), y, axis);

	UpdateWorldMatrix();
}

void Entity::SetRotZ(float z) {
	v3rot[2] = z;
	
	glm::vec3 axis;
	axis[0] = axis[1] = 0.0f;
	axis[2] = 1.0f;

	rotationZ = glm::rotate(glm::mat4(1.0f), z, axis);

	UpdateWorldMatrix();
}

void Entity::Rotate(float x, float y, float z) {
	v3rot[0] += x;
	v3rot[1] += y;
	v3rot[2] += z;

	rotMatrix = glm::rotate(glm::mat4(1.0f), v3rot[0], glm::vec3(1.0f, 0.0f, 0.0f));
	rotMatrix *= glm::rotate(glm::mat4(1.0f), v3rot[1], glm::vec3(0.0f, 1.0f, 0.0f));
	rotMatrix *= glm::rotate(glm::mat4(1.0f), v3rot[2], glm::vec3(0.0f, 0.0f, 1.0f));

	UpdateWorldMatrix();
}

void Entity::TranslationBox(float x, float y, float z) {									// Usamos translation para mover a la entidad porque sino se pierde la posicion original ya que la estoy pisando todo el tiempo.
	if (box == NULL) {																		// No hay caja de colision
		v3pos[0] += x;
		v3pos[1] += y;
		v3pos[2] += z;
	}
	else if (!box->GetCollision()) {														// Hay caja de colision, verifica el resultado del getter de colision de la caja
		v3pos[0] += x;
		v3pos[1] += y;
		v3pos[2] += z;
		box->SetPos(v3pos[0], v3pos[1]);
		box->SetCollision(false);
	}
	translateMatrix = glm::translate(glm::mat4(1.0f), v3pos);
	UpdateWorldMatrix();
}

void Entity::TranslationCircle(float x, float y, float z) {
	if (circle == NULL) {																	// No hay circunferencia de colision
		v3pos[0] += x;
		v3pos[1] += y;
		v3pos[2] += z;
	}
	else if (!circle->GetCollision()) {														// Hay circunferencia de colision, verifica el resultado del getter de colision de la caja
		v3pos[0] += x;
		v3pos[1] += y;
		v3pos[2] += z;
		circle->SetPos(v3pos[0], v3pos[1]);
		circle->SetCollision(false);
	}
	translateMatrix = glm::translate(glm::mat4(1.0f), v3pos);
	UpdateWorldMatrix();
}

void Entity::SetBoundingBox(float w, float h, float mass, bool setStatic, bool setTrigger) {
	box = new BoundingBox(glm::vec2(v3pos.x, v3pos.y), w, h, mass, setStatic, setTrigger);	// Seteamos los valores que va a tener la caja de colision de una entidad en particular
}

void Entity::SetBoundingCircle(float r, float mass, bool setStatic, bool setTrigger) {
	circle = new BoundingCircle(glm::vec2(v3pos.x, v3pos.y), r, mass, setStatic, setTrigger);// Seteamos los valores que va a tener la circunferencia de colision de una entidad en particular
}

void Entity::UpdateWorldMatrix() {
	worldMatrix = translateMatrix * rotMatrix * scaleMatrix;								// Multiplico las matrices luego de hacerles transformaciones
}

void Entity::SetRotationMatrix(float x, float y, float z, float w)
{
	float pitch, yaw, roll;

	glm::vec4 quaternion = glm::normalize(glm::vec4(x,y,z,w));

	ConvertToEulerAngles(quaternion, pitch, yaw, roll);

	v3rot = glm::vec3(pitch, yaw, roll);

	glm::mat4 mat1 = glm::mat4
	{
		quaternion.w, quaternion.z, -quaternion.y, quaternion.x,
		-quaternion.z, quaternion.w, quaternion.x, quaternion.y,
		quaternion.y, -quaternion.x, quaternion.w, quaternion.z,
		-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w
	};

	glm::mat4 mat2 = glm::mat4
	{
		quaternion.w, quaternion.z, -quaternion.y, -quaternion.x,
		-quaternion.z, quaternion.w, quaternion.x, -quaternion.y,
		quaternion.y, -quaternion.x, quaternion.w, -quaternion.z,
		quaternion.x, quaternion.y, quaternion.z, quaternion.w
	};

	rotMatrix = mat1 * mat2;

	ClampEulerRotation();
	UpdateWorldMatrix();
}

void Entity::ConvertToEulerAngles(const glm::vec4& quaternion, float& pitch, float& yaw, float& roll)
{
	float sinPitchCosYaw = 2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
	float cosPitchCosYaw = 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
	pitch = glm::atan(sinPitchCosYaw, cosPitchCosYaw);

	float sinYaw = 2.0f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x);
	yaw = (glm::abs(sinYaw) >= 1.0f) ? glm::sign(sinYaw) * glm::half_pi<float>() : glm::asin(sinYaw);

	float sinRollCosYaw = 2.0f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
	float cosRollCosYaw = 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	roll = glm::atan(sinRollCosYaw, cosRollCosYaw);

	pitch = glm::degrees(pitch);
	yaw = glm::degrees(yaw);
	roll = glm::degrees(roll);
}

void Entity::ClampEulerRotation()
{
	if (v3rot.x < 0.0f || v3rot.x >= FULL_ROTATION)
		v3rot.x = v3rot.x - (glm::floor(v3rot.x / FULL_ROTATION) * FULL_ROTATION);

	if (v3rot.y < 0.0f || v3rot.y >= FULL_ROTATION)
		v3rot.y = v3rot.y - (glm::floor(v3rot.y / FULL_ROTATION) * FULL_ROTATION);

	if (v3rot.z < 0.0f || v3rot.z >= FULL_ROTATION)
		v3rot.z = v3rot.z - (glm::floor(v3rot.z / FULL_ROTATION) * FULL_ROTATION);
}