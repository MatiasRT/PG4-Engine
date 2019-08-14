#include <Gamebase.h>
#include <CollisionManager.h>
#include <Triangle.h>
#include <Material.h>
#include <Rectangle.h>
#include <Circle.h>
#include <Importer.h>
#include <Sprite.h>
#include <Tilemap.h>
#include <Camera.h>
#include <Input.h>
#include <Mesh.h>
#include <iostream>
class Game : public Gamebase {
	int i;
	float rotation;
	float translation;
	float speed;

	Material * mat1;
	Material * mat2;
	Material * mat3;

	Mesh * mesh;
	Mesh * mesh2;

	GameNode * sceneNode;

	GameNode * snFirstChild;
	GameNode * snSecondChild;

	GameNode * scFirstChild;
	GameNode * scSecondChild;

	Camera * camera;
public:
	Game() { i = 0; };
	~Game() { };
protected:
	bool OnStart() override;
	bool OnStop() override;
	bool OnUpdate() override;
	void OnDraw() override { };
};