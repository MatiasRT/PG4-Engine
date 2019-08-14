#pragma once
#ifndef GAMENODE_H
#define GAMENODE_H
#include "Renderer.h"
#include "Exports.h"
#include "Component.h"
#include "Definitions.h"
#include <list>
using namespace std;

class ENGINEDLL_API GameNode : public Entity {
	Renderer * renderer;

	vector<Component*>* components;
	vector<GameNode*>* nodes;
public:
	GameNode(Renderer * render);
	~GameNode() { };

	void Update();
	void Draw();

	void AddComponent(Component * comp);
	void AddChild(GameNode * node);
	
	void RemoveChild(int index);
	void RemoveComponent(int index);

	vector<GameNode*>* GetChilds();

	GameNode * GetNode(int index);
	Component* GetComponent(ComponentsType type);
};
#endif