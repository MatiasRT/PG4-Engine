#include "Game.h"

bool Game::OnStart() {
	i = 0;
	translation = 0;
	rotation = 0;
	speed = 50.0f;

	camera = new Camera(renderer);
	Input* input = Input::Instance();
	input->SetWindow(GetWindow());

	/*mat1 = new Material();																	// Creo un Material
	unsigned int programID = mat1->LoadShaders("VertexColor.glsl", "FragmentColor.glsl");		// Le digo al Material cuales van a ser los shaders que tiene que utilizar. El VS se ejecuta una vez x cada pixel, y el FS se ejecuta una vez x muestra

	mat2 = new Material();
	unsigned int textureID = mat2->LoadShaders("VertexTexture.glsl", "FragmentTexture.glsl");

	mat3 = new Material();
	unsigned int tileID = mat3->LoadShaders("VertexTexture.glsl", "FragmentTexture.glsl");*/

	scene = new GameNode(renderer);															// Creo el Nodo Padre que va a contener todo
	cameraNode = new GameNode(renderer);														// Creo un Nodo hijo del Nodo Padre
	objects = new GameNode(renderer);														// Creo un Nodo hijo del Nodo Padre

	wall1 = new GameNode(renderer);
	wall2 = new GameNode(renderer);
	
	rifle = new GameNode(renderer);														// Creo un Nodo hijo del Segundo hijo del Nodo Padre
	//scSecondChild = new GameNode(renderer);														// Creo un Nodo hijo del Segundo hijo del Nodo Padre

	scene->AddChild(cameraNode);															// Le agrego un hijo al Nodo Padre
	scene->AddChild(objects);															// Le agrego un hijo al Nodo Padre
	//snSecondChild->AddChild(scFirstChild);														// Le agrego un hijo al segundo hijo del Nodo Padre
	//snSecondChild->AddChild(scSecondChild);														// Le agrego un hijo al segundo hijo del Nodo Padre

	cameraNode->AddComponent(camera);															// Le agrego un componente de camara al primer hijo del Nodo Padre

	//Importer::LoadMesh("glock.fbx", "glock.bmp", scFirstChild, renderer, camera);				// Cargo el modelo
	Importer::LoadMesh("BSPFinal.fbx", "pastote.bmp", objects, renderer, camera);				// Cargo el modelo
	camera->SetPos(0.0f, 0.0f, -10.0f);

	rifle = objects->GetNode(10);

	wall1 = objects->GetNode(5);
	wall2 = objects->GetNode(4);

	wall1->SetPos(10000, -50000, 0);
	wall2->SetPos(10000, -50000, 0);

	//snSecondChild->SetScale(0.001f, 0.001f, 0.001f);

	//scFirstChild->SetScale(0.5f, 0.5f, 0.5f);
	//scFirstChild->SetPos(0.0f, 0.0f, 20.0f);

	//scSecondChild->SetPos(4.0f, 0.0f, 60.0f);
	//scSecondChild->Rotate(0.0f, 2.0f, 0.0f);

	SetSceneNode(scene);																	// Seteo que es el Nodo Padre

	cout<<"Game::OnStart()"<<endl;
	return true;
}

bool Game::OnUpdate() {																			// Toda la logica va aca
	Input* input = Input::Instance();
	//CollisionManager::Instance()->BoxCollisionDetector();
	input->PollEvents();
	//speed = 20.0f;

	/* NODOS */
	//scFirstChild->Rotate(0, time * speed, 0);													// Rotacion de todo el modelo de la pistola con las balas y el cargador
	//scFirstChild->GetNode(3)->Rotate(time, 0, 0);												// Rotacion del cargador
	//scFirstChild->GetNode(4)->Rotate(time, 0, 0);												// Rotacion de bala n1
	//scFirstChild->GetNode(5)->Rotate(-time, 0, 0);												// Rotacion de bala n2
	//scFirstChild->GetNode(6)->Rotate(0, 0, time);												// Rotacion de bala n3

	//cout << "" << endl;

	/* MOVIMIENTO CAMARA */
	if (input->GetInput(GLFW_KEY_W))
		camera->Walk(0, 0.4f * speed * time);
	if (input->GetInput(GLFW_KEY_S))
		camera->Walk(0, -0.4f * speed * time);
	if (input->GetInput(GLFW_KEY_A))
		camera->Walk(0.4f * speed * time * 0.5f, 0);
	if (input->GetInput(GLFW_KEY_D))
		camera->Walk(-0.4f * speed * time * 0.5f, 0);

	/* ROTACION CAMARA */
	if (input->GetInput(GLFW_KEY_UP))
		camera->Pitch(-0.05f);
	if (input->GetInput(GLFW_KEY_DOWN))
		camera->Pitch(0.05f);
	if (input->GetInput(GLFW_KEY_LEFT))
		camera->Yaw(0.05f);
	if (input->GetInput(GLFW_KEY_RIGHT))
		camera->Yaw(-0.05f);
	if (input->GetInput(GLFW_KEY_Q))
		camera->Roll(-0.05f);
	if (input->GetInput(GLFW_KEY_E))
		camera->Roll(0.05f);

	/* SPRINT */
	if (input->GetInput(GLFW_KEY_LEFT_SHIFT))
		speed = 400.0f;
	else speed = 50.0f;

	/* RIFLE */
	if (input->GetInput(GLFW_KEY_Z))
		rifle->TranslationBox(0, 0,speed * time);

	if (input->GetInput(GLFW_KEY_C))
		rifle->TranslationBox(0, 0,speed * time * -1);

	return true;
}

bool Game::OnStop() {
	delete scene;
	delete cameraNode;
	delete objects;
	//delete scFirstChild;
	//delete scSecondChild;
	delete rifle;
	delete camera;
	cout << "Game::OnStop()" << endl;
	return false;
}