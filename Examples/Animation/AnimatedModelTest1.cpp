#include "AnimatedModelTest1.h"

#include "../TestHelper.h"
#include "../InputHelper.h"
#include "Andromeda/FileSystem/FileManager.h"
#include "Andromeda/Graphics/ShaderManager.h"

void AnimatedModelTest1::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//cam
	_cam = new Camera3d(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	//load shader
	_shader_gpu = _shaderManager->LoadFromFile("skinned_gpu_color", "Assets/Shaders/skinned_gpu_color", "Assets/Shaders/lit_color", NormalTextureWeighJoint);

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
	std::string modelFile = Andromeda::FileSystem::FileManager::Instance()->GetMainDirPath() + "Assets/Animation/Ducky.gltf";

	_animatedModel = new AnimatedModel();
	_animatedModel->SetSkinningType(SkinningType::GPU);
	_animatedModel->LoadAnimatedModel(modelFile);
	_animatedModel->SetShader(_shader_gpu);
}

void AnimatedModelTest1::Enter()
{

}

void AnimatedModelTest1::CleanUp()
{
	delete _cam;
	delete _timer;
}

void AnimatedModelTest1::Pause()
{

}

void AnimatedModelTest1::Resume()
{

}

void AnimatedModelTest1::GamePause()
{

}

void AnimatedModelTest1::GameResume()
{

}

void AnimatedModelTest1::HandleEvents(GameManager* manager)
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

void AnimatedModelTest1::Update(GameManager* manager)
{
	_dt = _timer->GetDelta();

	_animatedModel->Update(_dt);
}

void AnimatedModelTest1::Draw(GameManager* manager)
{
	//start frame
	_renderManager->StartFrame();

	//clear screen
	_renderManager->ClearScreen();

	//use texture
	_renderManager->UseTexture(_texture);

	//use shader
	_shader_gpu->Bind();

	glm::mat4 model{ 1.0f };
	glm::mat4 view{ 1.0f };
	glm::mat4 mvp{ 1.0f };
	glm::vec3 lit(1, 1, 1);

	//get view matrix from camera
	view = _cam->GetViewMatrix();
	mvp = _projection * view * model;

	glm::vec3 viewPosition = _cam->Position;


	glm::vec3 lampPosition = glm::vec3(0.0f, 5.0f, 5.0f);


	_shader_gpu->SetUniform(VertexShader, "model", model);
	_shader_gpu->SetUniform(VertexShader, "mvp", mvp);

	_shader_gpu->SetUniform(FragmentShader, "viewPos", viewPosition);
	_shader_gpu->SetUniform(FragmentShader, "light", lampPosition);


	_animatedModel->Draw();

	//draw test info
	TestHelper::Instance()->AddInfoText("Animated model test 1.");
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();
}