#include "TestAnimation3.h"

#include "../TestHelper.h"
#include "../InputHelper.h"
#include "Andromeda/FileSystem/FileManager.h"
#include "Andromeda/Graphics/ShaderManager.h"
#include "Andromeda/Graphics/Animation/GLTFLoader.h"
#include "Andromeda/Graphics/Animation/RearrangeBones.h"

void TestAnimation3::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//cam
	_cam = new Camera3d(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	//load shader
	_shader_gpu = _shaderManager->LoadFromFile("skinned_gpu_one", "Assets/Shaders/skinned_gpu_one", "Assets/Shaders/lit", NormalTextureWeighJoint);

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
	std::string modelFile = Andromeda::FileSystem::FileManager::Instance()->GetMainDirPath() + "Assets/Animation/nate2.gltf";
	cgltf_data* gltf = LoadGLTFFile(modelFile.c_str());
	mMeshes = LoadAnimationMeshes(gltf);
	mSkeleton = LoadSkeleton(gltf);
	mClips = LoadAnimationClips(gltf);
	FreeGLTFFile(gltf);

	BoneMap bones = RearrangeSkeleton(mSkeleton);
	for (unsigned int i = 0, size = (unsigned int)mMeshes.size(); i < size; ++i) {
		RearrangeMesh(mMeshes[i], bones);
	}
	for (unsigned int i = 0, size = (unsigned int)mClips.size(); i < size; ++i) {
		RearrangeClip(mClips[i], bones);
	}

	//update meshes
	for (unsigned int i = 0, size = (unsigned int)mMeshes.size(); i < size; ++i)
	{
		mMeshes[i].CreateMesh(SkinningType::GPU);
	}

	mCurrentClip = 0;
	mCurrentPose = mSkeleton.GetRestPose();


	mSkinType = SkinningType::GPU;
}

void TestAnimation3::Enter()
{

}

void TestAnimation3::CleanUp()
{
	delete _cam;
	delete _timer;
}

void TestAnimation3::Pause()
{

}

void TestAnimation3::Resume()
{

}

void TestAnimation3::GamePause()
{

}

void TestAnimation3::GameResume()
{

}

void TestAnimation3::HandleEvents(GameManager* manager)
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

void TestAnimation3::Update(GameManager* manager)
{
	_dt = _timer->GetDelta();


	mPlaybackTime = mClips[mCurrentClip].Sample(mCurrentPose, mPlaybackTime + _dt);
	mCurrentPose.GetMatrixPalette(mPosePalette);

	std::vector<AnimMat4>& invBindPose = mSkeleton.GetInvBindPose();
	for (unsigned int i = 0, size = (unsigned int)mPosePalette.size(); i < size; ++i) {
		mPosePalette[i] = mPosePalette[i] * invBindPose[i];
	}
}

void TestAnimation3::Draw(GameManager* manager)
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

	_shader_gpu->SetUniform(VertexShader, "model", model);
	_shader_gpu->SetUniform(VertexShader, "mvp", mvp);
	_shader_gpu->SetUniform(FragmentShader, "light", lit);
	_shader_gpu->Set(VertexShader, "pose", mPosePalette);

	for (unsigned int i = 0, size = mMeshes.size(); i < size; ++i) 
	{
		mMeshes[i].Draw();
	}

	//draw test info
	TestHelper::Instance()->AddInfoText("Skinning (GPU) Gltf model test.");
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();
}