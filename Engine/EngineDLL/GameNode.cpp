#include "GameNode.h"

GameNode::GameNode(Renderer * render) : Entity(render) {
	renderer = render;
	worldMatrix = renderer->GetViewMatrix();
	nodes = new vector<GameNode*>();
	components = new vector<Component*>();
}

void GameNode::Update() {
	for (int i=0; i<components->size(); i++)
		components->at(i)->Update();

	for (int j=0; j<nodes->size(); j++)
		nodes->at(j)->Update();
}

void GameNode::Draw() {
	//me guardo la MVP anterior
	glm::mat4 actualWM = renderer->GetWorldMatrix();
	glm::mat4 actualVM = renderer->GetViewMatrix();
	glm::mat4 actualPM = renderer->GetProjectionMatrix();

	//multiplico
	renderer->SetWorldMatrix(actualWM * worldMatrix);

	for (int i = 0; i < components->size(); i++)
		components->at(i)->Draw();

	for (int j = 0; j < nodes->size(); j++)
		nodes->at(j)->Draw();

	//las vuelvo a setear
	renderer->SetViewMatrix(actualVM);
	renderer->SetWorldMatrix(actualWM);
	renderer->SetProjectionMatrix(actualPM);
}

void GameNode::AddComponent(Component * comp) {
	components->push_back(comp);
}

void GameNode::RemoveComponent(int index) {
	int i = 0;
	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) {
		if (i == index) {
			components->erase(it);
			break;
		}
		i++;
	}
}

void GameNode::RemoveChild(int index) {
	int i = 0;
	for (vector<GameNode*>::iterator it = nodes->begin(); it != nodes->end(); ++it) {
		if (i == index) {
			nodes->erase(it);
			break;
		}
		i++;
	}
}

void GameNode::AddChild(GameNode * node) {
	nodes->push_back(node);
}

vector<GameNode*>* GameNode::GetChilds() {
	if (nodes == nullptr || nodes->size() == 0)
		return nullptr;
	return nodes;
}

GameNode * GameNode::GetNode(int index) {
	if (index >= nodes->size())
		return nodes->at(nodes->size() - 1);
	return nodes->at(index);
}

Component * GameNode::GetComponent(ComponentsType type) {
	for (size_t i = 0; i < components->size(); i++) {
		if (components->at(i)->type == type)
			return components->at(i);
	}
}