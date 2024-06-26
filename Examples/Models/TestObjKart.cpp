

#include "TestObjKart.h"

#include "../TestHelper.h"
#include "../InputHelper.h"


void TestObjKart::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//load shader
	_shader = _shaderManager->LoadFromFile("simple", "Assets/Shaders/vertex_color_texture_transform_3d", "Assets/Shaders/vertex_color_texture", TextureColor);

	//load car model
	_carModel = new ModelObj();
	//_carModel->ConvertToBinary("Assets/Models/Obj/WillyKart/willy.obj", "Assets/Models/Obj/WillyKart/willy.objb");
	_carModel->LoadBinary("Assets/Models/Obj/WillyKart/willy.objb");
	_carModel->SetShader(_shader);

	//load wheel model
	_wheelModel = new ModelObj();
	//_wheelModel->ConvertToBinary("Assets/Models/Obj/WillyKart/armywheels.obj", "Assets/Models/Obj/WillyKart/armywheels.objb");
	_wheelModel->LoadBinary("Assets/Models/Obj/WillyKart/armywheels.objb");
	_wheelModel->SetShader(_shader);

	//wheel positions	
	_wheelPositions.push_back(glm::vec3(-0.33f, -0.17f, 0.335f));
	_wheelPositions.push_back(glm::vec3(-0.33f, -0.17f, -0.26f));

	_wheelPositions.push_back(glm::vec3(0.33f, -0.17f, 0.335f));
	_wheelPositions.push_back(glm::vec3(0.33f, -0.17f, -0.26f));

	//cam
	_cam = new Camera3d(glm::vec3(0.0f, 1.0f, 4.5f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	//
	_projection = glm::perspective(45.0f, (float)_renderManager->GetWidth() / (float)_renderManager->GetHeight(), 0.1f, 100.0f);

	//input
	_inputManager = InputManager::Instance();

	if (_inputManager->GetKayboardCount() > 0)
		_keyboard = _inputManager->GetKeyboardDevice(0);

	if (_inputManager->GetMouseCount() > 0)
	{
		_mouse = _inputManager->GetMouseDevice(0);

		//disable cursor
		_mouse->SetCursorVisible(false);

		_useMouse = true;
		_firstMouse = true;
	}

	if (_inputManager->GetGamepadCount() > 0)
		_gamepad = _inputManager->GetGamepadDevice(0);

	_timer = new Timer();
}

void TestObjKart::Enter()
{

}

void TestObjKart::CleanUp()
{
	delete _carModel;
	delete _wheelModel;

	delete _cam;
	delete _timer;

	_shaderManager->Remove(_shader);
}

void TestObjKart::Pause()
{

}

void TestObjKart::Resume()
{

}

void TestObjKart::GamePause()
{

}

void TestObjKart::GameResume()
{

}

void TestObjKart::HandleEvents(GameManager* manager)
{
	if (_mouse != 0 && _useMouse)
	{
		int posx = _mouse->GetPosX();
		int posy = _mouse->GetPosY() * -1.0f;

		if (_firstMouse)
		{
			moveX = posx;
			moveY = posy;

			_firstMouse = false;
		}

		int xoffset = posx - moveX;
		int yoffset = posy - moveY;

		moveX = posx;
		moveY = posy;

		_cam->ProcessMouseMovement(xoffset, yoffset, false);
	}


	if (_keyboard != 0)
	{
		//update cam
		if (_keyboard->KeyDown(Key::W))
			_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::FORWARD, _dt);
		if (_keyboard->KeyDown(Key::S))
			_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::BACKWARD, _dt);
		if (_keyboard->KeyDown(Key::A))
			_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::LEFT, _dt);
		if (_keyboard->KeyDown(Key::D))
			_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::RIGHT, _dt);

		if (_keyboard->KeyDown(Key::Left))
			_cam->ProcessMouseMovement(-5, 0, false);
		if (_keyboard->KeyDown(Key::Right))
			_cam->ProcessMouseMovement(5, 0, false);
		if (_keyboard->KeyDown(Key::Up))
			_cam->ProcessMouseMovement(0, 5, false);
		if (_keyboard->KeyDown(Key::Down))
			_cam->ProcessMouseMovement(0, -5, false);
	}

	if (_gamepad != 0)
	{
		//update cam
		if (_gamepad->KeyDown(Gamepad::Up))
			_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::FORWARD, _dt);
		if (_gamepad->KeyDown(Gamepad::Down))
			_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::BACKWARD, _dt);
		if (_gamepad->KeyDown(Gamepad::Left))
			_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::LEFT, _dt);
		if (_gamepad->KeyDown(Gamepad::Right))
			_cam->ProcessKeyboard(Camera3d::CameraMovementEnum::RIGHT, _dt);

		if (_gamepad->KeyDown(Gamepad::Square))
			_cam->ProcessMouseMovement(-5, 0, false);
		if (_gamepad->KeyDown(Gamepad::Circle))
			_cam->ProcessMouseMovement(5, 0, false);
		if (_gamepad->KeyDown(Gamepad::Triangle))
			_cam->ProcessMouseMovement(0, 5, false);
		if (_gamepad->KeyDown(Gamepad::Cross))
			_cam->ProcessMouseMovement(0, -5, false);
	}

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

void TestObjKart::Update(GameManager* manager)
{
	_dt = _timer->GetDelta();
}

void TestObjKart::Draw(GameManager* manager)
{
	//start frame
	_renderManager->StartFrame();

	//clear screen
	_renderManager->ClearScreen();

	//use shader
	_shader->Bind();

	//draw car
	{
		glm::mat4 model{ 1.0 };;
		glm::mat4 view{ 1.0 };;
		glm::mat4 mvp{ 1.0 };;

		//get view matrix from camera
		view = _cam->GetViewMatrix();

		mvp = _projection * view * model;

		_shader->SetUniform(VertexShader, "mvp", mvp);

		_carModel->Draw();
	}

	//draw wheels
	for (int i = 0; i < _wheelPositions.size(); i++)
	{
		glm::mat4 model{ 1.0 };;
		glm::mat4 view{ 1.0 };;
		glm::mat4 mvp{ 1.0 };;

		model = glm::translate(model, _wheelPositions[i]);

		if (i > 1)
		{
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		//get view matrix from camera
		view = _cam->GetViewMatrix();

		mvp = _projection * view * model;

		_shader->SetUniform(VertexShader, "mvp", mvp);

		_wheelModel->Draw();
	}

	//draw test info
	TestHelper::Instance()->AddInfoText("Load OBJ Willy model.");
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();
}