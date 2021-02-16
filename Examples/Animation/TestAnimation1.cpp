#include "TestAnimation1.h"

#include "../TestHelper.h"
#include "../InputHelper.h"
#include "Andromeda/FileSystem/FileManager.h"
#include "Andromeda/Graphics/ShaderManager.h"
#include "Andromeda/Graphics/Animation/GLTFLoader.h"
#include "Andromeda/Utils/Logger.h"

void TestAnimation1::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//cam
	_cam = new Camera3d(glm::vec3(0.0f, 1.0f, 2.5f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	//load shader
	_shader = _shaderManager->LoadFromFile("static_model", "Assets/Shaders/static", "Assets/Shaders/lit", TextureNormal);

	//load texture
	_texture = _textureManager->LoadFromFile("Assets/Animation/texture.png");

	//
	_projection = glm::perspective(45.0f, (float)_renderManager->GetWidth() / (float)_renderManager->GetHeight(), 0.1f, 1000.0f);

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

	//
    Andromeda::Utils::Logger::Instance()->Log("Load gltf model\n");
	std::string modelFile = Andromeda::FileSystem::FileManager::Instance()->GetMainDirPath() + "Assets/Animation/nate2.gltf";
	cgltf_data* gltf = LoadGLTFFile(modelFile.c_str());
	animatedMeshes = LoadAnimationMeshes(gltf);
	FreeGLTFFile(gltf);

	Andromeda::Utils::Logger::Instance()->Log("Load gltf model done\n");

	for (unsigned int i = 0, size = (unsigned int)animatedMeshes.size(); i < size; ++i)
	{
		animatedMeshes[i].CreateMesh(SkinningType::None);
	}

	Andromeda::Utils::Logger::Instance()->Log("Mesh done\n");

}

void TestAnimation1::Enter()
{

}

void TestAnimation1::CleanUp()
{
	delete _cam;
	delete _timer;
}

void TestAnimation1::Pause()
{

}

void TestAnimation1::Resume()
{

}

void TestAnimation1::GamePause()
{

}

void TestAnimation1::GameResume()
{

}

void TestAnimation1::HandleEvents(GameManager* manager)
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

void TestAnimation1::Update(GameManager* manager)
{
	_dt = _timer->GetDelta();
}

void TestAnimation1::Draw(GameManager* manager)
{
	//start frame
	_renderManager->StartFrame();

	//clear screen
	_renderManager->ClearScreen();

	//use texture
	_renderManager->UseTexture(_texture);

	////use shader
	_shader->Bind();

	glm::mat4 model{ 1.0f };
	glm::mat4 view{ 1.0f };
	glm::mat4 mvp{ 1.0f };
	glm::vec3 light(1, 1, 1);

	////get view matrix from camera
	view = _cam->GetViewMatrix();

	mvp = _projection * view * model;

	_shader->SetUniform(VertexShader, "model", model);
	_shader->SetUniform(VertexShader, "mvp", mvp);
	_shader->SetUniform(FragmentShader, "light", light);

	for (unsigned int i = 0, size = (unsigned int)animatedMeshes.size(); i < size; ++i) 
	{
		animatedMeshes[i].Draw();
	}

	//draw test info
	TestHelper::Instance()->AddInfoText("Static Gltf model test.");
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();
}