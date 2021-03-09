#include "CharacterControllerTest.h"

#include "../TestHelper.h"
#include "../InputHelper.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void CharacterControllerTest::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//load shader
	_shader = _shaderManager->LoadFromFile("simple", "Assets/Shaders/vertex_color_texture_transform_3d", "Assets/Shaders/vertex_color_texture", TextureColor);

	_floorTexture = _textureManager->LoadFromFile("Assets/Images/wood.png", Andromeda::Graphics::TextureFilerType::LinearFilter, Andromeda::Graphics::TextureColorType::Texture_RGBA, Andromeda::Graphics::TextureWrapType::Repeat, 7);

	_soccerModel = new ModelObj();
	_soccerModel->ConvertToBinary("Assets/Models/Obj/capsule.obj","Assets/Models/Obj/capsule.objb");
	_soccerModel->LoadBinary("Assets/Models/Obj/capsule.objb");
	_soccerModel->SetShader(_shader);

	//cam
	_cam = new Camera3d(glm::vec3(0.0f, 4.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	//cam settings
	_cameraHeight = 3.0f;
	_minCameraDistance = 3.f;
	_maxCameraDistance = 5.f;

	//
	_projection = glm::perspective(45.0f, (float)_renderManager->GetWidth() / (float)_renderManager->GetHeight(), 0.1f, 100.0f);

	//input
	_inputManager = InputManager::Instance();

	if (_inputManager->GetKayboardCount() > 0)
		_keyboard = _inputManager->GetKeyboardDevice(0);

	if (_inputManager->GetMouseCount() > 0)
	{
		//disable cursor
		_inputManager->GetMouseDevice(0)->SetCursorVisible(false);
	}

	if (_inputManager->GetGamepadCount() > 0)
		_gamepad = _inputManager->GetGamepadDevice(0);

	InitModels();
	InitPhysic();

	_timer = new Timer();
}

void CharacterControllerTest::InitModels()
{
	//create floor model
	{
		//create data buffer object
		_floorModel = _renderManager->CreateVertexArrayObject(TextureColor, StaticDraw);

		//create vertices
		_floorModel->CreateVertices(6);

		//get vertices
		TextureColorVertex* _simpleData = static_cast<TextureColorVertex*>(_floorModel->GetVertices());

		//position
		_simpleData[0].x = -50.5f;	_simpleData[0].y = 0.0f;	_simpleData[0].z = -50.5f;
		_simpleData[1].x = 50.5f;	_simpleData[1].y = 0.0f;	_simpleData[1].z = -50.5f;
		_simpleData[2].x = 50.5f;	_simpleData[2].y = 0.0f;	_simpleData[2].z = 50.5f;
		_simpleData[3].x = 50.5f;	_simpleData[3].y = 0.0f;	_simpleData[3].z = 50.5f;
		_simpleData[4].x = -50.5f;	_simpleData[4].y = 0.0f;	_simpleData[4].z = 50.5f;
		_simpleData[5].x = -50.5f;	_simpleData[5].y = 0.0f;	_simpleData[5].z = -50.5f;

		//color
		for (int i = 0; i < 6; i++)
		{
			_simpleData[i].r = 1.0f;	_simpleData[i].g = 1.0f;	_simpleData[i].b = 1.0f;
		}

		//texture
		_simpleData[0].u = 0.0f;	_simpleData[0].v = 25.0f;
		_simpleData[1].u = 25.0f;	_simpleData[1].v = 25.0f;
		_simpleData[2].u = 25.0f;	_simpleData[2].v = 0.0f;
		_simpleData[3].u = 25.0f;	_simpleData[3].v = 0.0f;
		_simpleData[4].u = 0.0f;	_simpleData[4].v = 0.0f;
		_simpleData[5].u = 0.0f;	_simpleData[5].v = 25.0f;

		//create indices
		_floorModel->CreateIndices(6);

		//get indices
		unsigned short* _indices = static_cast<unsigned short*>(_floorModel->GetIndices());

		//set data
		for (int i = 0; i < 6; i++)
		{
			_indices[i] = i;
		}

		//generate buffer object
		_floorModel->Generate();
	}
}

void CharacterControllerTest::InitPhysic()
{
	btVector3 worldAabbMin(-1000, -1000, -1000);
	btVector3 worldAabbMax(1000, 1000, 1000);
	int maxProxies = 32766;

	//Collision configuration contains default setup for memory, collision setup
	physCollisionConfiguration = new btDefaultCollisionConfiguration();

	//Use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	physDispatcher = new btCollisionDispatcher(physCollisionConfiguration);
	physBroadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);


	//The default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	physSolver = solver;

	//Create the main physics object
	physDynamicsWorld = new btDiscreteDynamicsWorld(physDispatcher, physBroadphase, physSolver, physCollisionConfiguration);
	physDynamicsWorld->setGravity(btVector3(0, -10, 0));

	//Create the ground object
	{
		// Create the ground shape
		physGroundShape = new btBoxShape(btVector3(btScalar(50.0f), btScalar(0.5f), btScalar(50.0f)));

		// Set the position for the ground shape
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -0.5f, 0));

		// Create MotionState and RigidBody object for the ground shape
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);

		float mass = 0;
		btVector3 localInertia(0, 0, 0);

		physGroundShape->calculateLocalInertia(mass, localInertia);
		physGroundBody = new btRigidBody(mass, myMotionState, physGroundShape, localInertia);

		// Add ground body to physics object
		physDynamicsWorld->addRigidBody(physGroundBody);
	}

	//Create the box object
	{
		//Create the box shape
		physBallShape = new btCapsuleShape(0.5f, 1.9f);// btBoxShape(btVector3(0.5, 0.5, 0.5));

		//Set mass, initial inertia and position for the box
		float mass = 10.0f;
		btVector3 inertia(0, 0, 0);
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(0, 4, 0));

		//Calculate the inertia
		physBallShape->calculateLocalInertia(mass, inertia);

		// Create MotionState and RigidBody object for the box shape
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		physBallBody = new btRigidBody(mass, myMotionState, physBallShape, inertia);

		// Add box body to physics object & activate it
		physDynamicsWorld->addRigidBody(physBallBody);
		physBallBody->activate();
		physBallBody->setActivationState(DISABLE_DEACTIVATION);
		physBallBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
		//physBallBody->setFriction(0.0f);
		//physBallBody->setRollingFriction(0.0f);
		//physBallBody->setSpinningFriction(0.0f);
	}

	controller = new SimpleCharacterController(physBallBody, physBallShape);
}

glm::mat4 CharacterControllerTest::btScalar2glmMat4(btScalar* matrix)
{
	return glm::mat4(
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
}

void CharacterControllerTest::CleanUp()
{
	delete _soccerModel;
	delete _floorModel;

	delete _cam;
	delete _timer;

	//remove textures
	_textureManager->Remove(_floorTexture);

	//remove shader
	_shaderManager->Remove(_shader);


	//bullet
	physDynamicsWorld->removeRigidBody(physBallBody);
	physDynamicsWorld->removeRigidBody(physGroundBody);

	delete physBallBody->getMotionState();
	delete physGroundBody->getMotionState();

	delete physDynamicsWorld;
	delete physBroadphase;
	delete physDispatcher;
	delete physSolver;
	delete physCollisionConfiguration;

	delete physBallShape;
	delete physGroundShape;
	delete physGroundBody;
}

void CharacterControllerTest::Pause()
{

}

void CharacterControllerTest::Resume()
{

}

void CharacterControllerTest::GamePause()
{

}

void CharacterControllerTest::GameResume()
{

}

void CharacterControllerTest::HandleEvents(GameManager* manager)
{
	//if (_mouse != 0 && _useMouse)
	//{
	//	int posx = _mouse->GetPosX();
	//	int posy = _mouse->GetPosY() * -1.0f;

	//	if (_firstMouse)
	//	{
	//		moveX = posx;
	//		moveY = posy;

	//		_firstMouse = false;
	//	}

	//	int xoffset = posx - moveX;
	//	int yoffset = posy - moveY;

	//	moveX = posx;
	//	moveY = posy;

	//	_cam->ProcessMouseMovement(xoffset, yoffset, false);
	//}

	
	desiredVelocity = glm::vec3(0,0,0);


	if (_keyboard != 0)
	{
		//update cam
		if (_keyboard->KeyDown(Key::W))
			desiredVelocity.x = 1.0f;
		if (_keyboard->KeyDown(Key::S))
			desiredVelocity.x = -1.0f;
		//if (_keyboard->KeyDown(Key::A))
		//	_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::LEFT, _dt);
		//if (_keyboard->KeyDown(Key::D))
		//	_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::RIGHT, _dt);

		//if (_keyboard->KeyDown(Key::Left))
		//	_cam->ProcessMouseMovement(-5, 0, false);
		//if (_keyboard->KeyDown(Key::Right))
		//	_cam->ProcessMouseMovement(5, 0, false);
		//if (_keyboard->KeyDown(Key::Up))
		//	_cam->ProcessMouseMovement(0, 5, false);
		//if (_keyboard->KeyDown(Key::Down))
		//	_cam->ProcessMouseMovement(0, -5, false);
	}

	controller->setMovementDirection(btVector3(desiredVelocity.x, desiredVelocity.y, desiredVelocity.z));


	//if (_gamepad != 0)
	//{
	//	//update cam
	//	if (_gamepad->KeyDown(Gamepad::Up))
	//		_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::FORWARD, _dt);
	//	if (_gamepad->KeyDown(Gamepad::Down))
	//		_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::BACKWARD, _dt);
	//	if (_gamepad->KeyDown(Gamepad::Left))
	//		_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::LEFT, _dt);
	//	if (_gamepad->KeyDown(Gamepad::Right))
	//		_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::RIGHT, _dt);

	//	if (_gamepad->KeyDown(Gamepad::Square))
	//		_cam->ProcessMouseMovement(-5, 0, false);
	//	if (_gamepad->KeyDown(Gamepad::Circle))
	//		_cam->ProcessMouseMovement(5, 0, false);
	//	if (_gamepad->KeyDown(Gamepad::Triangle))
	//		_cam->ProcessMouseMovement(0, 5, false);
	//	if (_gamepad->KeyDown(Gamepad::Cross))
	//		_cam->ProcessMouseMovement(0, -5, false);
	//}

	if (InputHelper::Instance()->ActionPressed(InputAction::Next))
	{
		TestHelper::Instance()->NextTest(manager);
	}

	if (InputHelper::Instance()->ActionPressed(InputAction::Exit))
	{
		manager->Quit();
	}

	InputHelper::Instance()->Update();
}


void CharacterControllerTest::Update(GameManager* manager)
{
	_dt = _timer->GetDelta();


	controller->updateAction(physDynamicsWorld, _dt);


	btScalar fixedTimeStep = 1. / 60.;

	//physic update
	if (physDynamicsWorld)
		physDynamicsWorld->stepSimulation(_dt);




}

void CharacterControllerTest::Draw(GameManager* manager)
{
	//start frame
	_renderManager->StartFrame();

	//clear screen
	_renderManager->ClearScreen();

	//use shader
	_shader->Bind();

	//use texture
	_renderManager->UseTexture(_floorTexture);

	//camera logic
	glm::vec3 cameraTargetPosition = glm::vec3(physBallBody->getWorldTransform().getOrigin()[0], physBallBody->getWorldTransform().getOrigin()[1], physBallBody->getWorldTransform().getOrigin()[2]);

	_cam->Position.y = (15.0*_cam->Position.y + cameraTargetPosition.y + _cameraHeight) / 16.0;

	glm::vec3 camToObject = cameraTargetPosition - _cam->Position;

	float cameraDistance = glm::length(camToObject);
	float correctionFactor = 0.f;

	if (cameraDistance < _minCameraDistance)
	{
		correctionFactor = 0.15*(_minCameraDistance - cameraDistance) / cameraDistance;
	}
	if (cameraDistance > _maxCameraDistance)
	{
		correctionFactor = 0.15*(_maxCameraDistance - cameraDistance) / cameraDistance;
	}

	_cam->Position.x -= (correctionFactor * camToObject).x;
	_cam->Position.y -= (correctionFactor * camToObject).y;
	_cam->Position.z -= (correctionFactor * camToObject).z;

	//cam view
	glm::mat4 camView = glm::lookAt(_cam->Position, cameraTargetPosition, _cam->Up);

	//draw floor
	{
		glm::mat4 model{ 1.0 };;
		glm::mat4 mvp{ 1.0 };;

		//
		mvp = _projection * camView * model;

		_shader->SetUniform(VertexShader, "mvp", mvp);

		_floorModel->Draw();
	}

	// draw ball
	{
		glm::mat4 model{ 1.0 };;
		glm::mat4 mvp{ 1.0 };;

		//get box matrix from bullet object
		float cubeMatrix[16];
		btTransform cubeTransform;
		physBallBody->getMotionState()->getWorldTransform(cubeTransform);
		cubeTransform.getOpenGLMatrix((btScalar*)&cubeMatrix);

		model = btScalar2glmMat4(cubeMatrix);

		//
		mvp = _projection * camView * model;

		_shader->SetUniform(VertexShader, "mvp", mvp);

		_soccerModel->Draw();
	}


	//draw test info
	TestHelper::Instance()->AddInfoText("Ball test");
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();
}