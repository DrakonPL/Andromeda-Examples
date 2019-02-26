

#include "TestMd5Model.h"

#include "../TestHelper.h"
#include "../InputHelper.h"


void TestMd5Model::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//load shader
	_shader = _shaderManager->LoadFromFile("model3d", "Assets/Shaders/vertex_color_texture_transform_3d", "Assets/Shaders/vertex_color_texture", TextureColor);

	_model = new ModelMd5();
	_model->LoadModel("Assets/Models/Md5/hellknight/hellknight.md5mesh");
	_model->LoadAnim("Assets/Models/Md5/hellknight/stand.md5anim");

	//cam
	_cam = new Camera3d(glm::vec3(0.0f, 1.5f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	//
	_projection = glm::perspective(45.0f, (float)_renderManager->GetWidth() / (float)_renderManager->GetHeight(), 0.1f, 400.0f);

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

	screen = 0;

	_timer = new Timer();
}

void TestMd5Model::Enter()
{

}

void TestMd5Model::CleanUp()
{
	delete _model;
	delete _cam;
	delete _timer;

	_shaderManager->Remove(_shader);
}

void TestMd5Model::Pause()
{

}

void TestMd5Model::Resume()
{

}

void TestMd5Model::GamePause()
{

}

void TestMd5Model::GameResume()
{

}

void TestMd5Model::HandleEvents(GameManager* manager)
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

void TestMd5Model::Update(GameManager* manager)
{
	_dt = _timer->GetDelta();
	_model->Update(_dt);

	if (screen == 1)
	{
		_renderManager->SaveDisplayBuffer("test.tga");

		//screen = 2;
	}
}

void TestMd5Model::Draw(GameManager* manager)
{
	//start frame
	_renderManager->StartFrame();

	//clear screen
	_renderManager->ClearScreen();

	//use texture
	//_renderManager->UseTexture(_texture);

	//use shader
	_shader->Bind();

	//draw imrod
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 mvp;

		//get view matrix from camera
		view = _cam->GetViewMatrix();

		model = glm::scale(glm::vec3(0.025f, 0.025f, 0.025f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		mvp = _projection * view * model;

		_shader->SetUniform(VertexShader, "mvp", mvp);

		_model->Render();
	}

	//draw test info
	TestHelper::Instance()->AddInfoText("Load Md5 hellknight model.");
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();

	if (screen == 0)
	{
		screen = 1;
	}

}