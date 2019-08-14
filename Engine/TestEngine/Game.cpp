#include "Game.h"

bool Game::OnStart() {
	i = 0;
	translation = 0;
	rotation = 0;
	speed = 0;

	camera = new Camera(renderer);
	Input* input = Input::Instance();
	input->SetWindow(GetWindow());

	/*mat1 = new Material();																	// Creo un Material
	unsigned int programID = mat1->LoadShaders("VertexColor.glsl", "FragmentColor.glsl");		// Le digo al Material cuales van a ser los shaders que tiene que utilizar. El VS se ejecuta una vez x cada pixel, y el FS se ejecuta una vez x muestra

	mat2 = new Material();
	unsigned int textureID = mat2->LoadShaders("VertexTexture.glsl", "FragmentTexture.glsl");

	mat3 = new Material();
	unsigned int tileID = mat3->LoadShaders("VertexTexture.glsl", "FragmentTexture.glsl");*/

	sceneNode = new GameNode(renderer);															// Creo el Nodo Padre que va a contener todo
	snFirstChild = new GameNode(renderer);														// Creo un Nodo hijo del Nodo Padre
	snSecondChild = new GameNode(renderer);														// Creo un Nodo hijo del Nodo Padre
	scFirstChild = new GameNode(renderer);														// Creo un Nodo hijo del Segundo hijo del Nodo Padre
	scSecondChild = new GameNode(renderer);														// Creo un Nodo hijo del Segundo hijo del Nodo Padre

	sceneNode->AddChild(snFirstChild);															// Le agrego un hijo al Nodo Padre
	sceneNode->AddChild(snSecondChild);															// Le agrego un hijo al Nodo Padre
	snSecondChild->AddChild(scFirstChild);														// Le agrego un hijo al segundo hijo del Nodo Padre
	snSecondChild->AddChild(scSecondChild);														// Le agrego un hijo al segundo hijo del Nodo Padre

	snFirstChild->AddComponent(camera);															// Le agrego un componente de camara al primer hijo del Nodo Padre

	Importer::LoadMesh("glock.fbx", "glock.bmp", scFirstChild, renderer, camera);				// Cargo el modelo
	Importer::LoadMesh("sword.fbx", "sword.bmp", scSecondChild, renderer, camera);				// Cargo el modelo
	
	scFirstChild->SetScale(0.5f, 0.5f, 0.5f);
	scFirstChild->SetPos(0.0f, 0.0f, 20.0f);

	scSecondChild->SetPos(4.0f, 0.0f, 60.0f);
	scSecondChild->Rotate(0.0f, 2.0f, 0.0f);

	SetSceneNode(sceneNode);																	// Seteo que es el Nodo Padre

	cout<<"Game::OnStart()"<<endl;
	return true;
}

bool Game::OnUpdate() {																			// Toda la logica va aca
	Input* input = Input::Instance();
	//CollisionManager::Instance()->BoxCollisionDetector();
	input->PollEvents();
	speed = 0.2f;

	/* NODOS */
	scFirstChild->Rotate(0, time * speed, 0);													// Rotacion de todo el modelo de la pistola con las balas y el cargador
	scFirstChild->GetNode(3)->Rotate(time, 0, 0);												// Rotacion del cargador
	scFirstChild->GetNode(4)->Rotate(time, 0, 0);												// Rotacion de bala n1
	scFirstChild->GetNode(5)->Rotate(-time, 0, 0);												// Rotacion de bala n2
	scFirstChild->GetNode(6)->Rotate(0, 0, time);												// Rotacion de bala n3


	/* MOVIMIENTO CAMARA */
	if (input->GetInput(GLFW_KEY_W))
		camera->Walk(0, 0.1f);
	if (input->GetInput(GLFW_KEY_S))
		camera->Walk(0, -0.1f);
	if (input->GetInput(GLFW_KEY_A))
		camera->Walk(0.4f, 0);
	if (input->GetInput(GLFW_KEY_D))
		camera->Walk(-0.4f, 0);								

	/* ROTACION CAMARA */
	if (input->GetInput(GLFW_KEY_UP))
		camera->Pitch(-0.01f);
	if (input->GetInput(GLFW_KEY_DOWN))
		camera->Pitch(0.01f);
	if (input->GetInput(GLFW_KEY_LEFT))
		camera->Yaw(0.01f);
	if (input->GetInput(GLFW_KEY_RIGHT))
		camera->Yaw(-0.01f);
	if (input->GetInput(GLFW_KEY_Q))
		camera->Roll(-0.01f);
	if (input->GetInput(GLFW_KEY_E))
		camera->Roll(0.01f);


	return true;
}

bool Game::OnStop() {
	delete sceneNode;
	delete snFirstChild;
	delete snSecondChild;
	delete scFirstChild;
	delete scSecondChild;

	delete camera;
	cout << "Game::OnStop()" << endl;
	return false;
}