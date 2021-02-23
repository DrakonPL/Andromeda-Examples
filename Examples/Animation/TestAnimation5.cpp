#include "TestAnimation5.h"

#include "../TestHelper.h"
#include "../InputHelper.h"
#include "Andromeda/FileSystem/FileManager.h"
#include "Andromeda/Graphics/ShaderManager.h"
#include "Andromeda/Graphics/Animation/GLTFLoader.h"
#include "Andromeda/Graphics/Animation/RearrangeBones.h"

void TestAnimation5::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//cam
	_cam = new Camera3d(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	//load shader
	_shader_gpu = _shaderManager->LoadFromFile("skinned_gpu_color", "Assets/Shaders/skinned_gpu_color", "Assets/Shaders/lit_color_material", NormalTextureWeighJoint);

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

	mCurrentClip = 1;
	mCurrentPose = mSkeleton.GetRestPose();


	mSkinType = SkinningType::GPU;
}

void TestAnimation5::Enter()
{

}

void TestAnimation5::CleanUp()
{
	delete _cam;
	delete _timer;
}

void TestAnimation5::Pause()
{

}

void TestAnimation5::Resume()
{

}

void TestAnimation5::GamePause()
{

}

void TestAnimation5::GameResume()
{

}

void TestAnimation5::HandleEvents(GameManager* manager)
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

void TestAnimation5::Update(GameManager* manager)
{
	_dt = _timer->GetDelta();

	mPlaybackTime = mClips[mCurrentClip].Sample(mCurrentPose, mPlaybackTime + _dt);
	mCurrentPose.GetMatrixPalette(mPosePalette);
}

void TestAnimation5::Draw(GameManager* manager)
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


	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lampPosition = glm::vec3(0.0f, 5.0f, 5.0f);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.7f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.3f); // low influence
	glm::vec3 lispecular = glm::vec3(1.0f, 1.0f, 1.0f); 

	glm::vec3 matambient = glm::vec3(0.3f, 0.3f, 0.3f);
	glm::vec3 matspecular = glm::vec3(0.5f, 0.5f, 0.5f); 


	_shader_gpu->SetUniform(VertexShader, "model", model);
	_shader_gpu->SetUniform(VertexShader, "mvp", mvp);
	_shader_gpu->Set(VertexShader, "pose", mPosePalette);
	_shader_gpu->Set(VertexShader, "invBindPose", mSkeleton.GetInvBindPose());

	_shader_gpu->SetUniform(FragmentShader, "liposition", lampPosition);
	_shader_gpu->SetUniform(FragmentShader, "viewPos", viewPosition);

	_shader_gpu->SetUniform(FragmentShader, "liambient", ambientColor);
	_shader_gpu->SetUniform(FragmentShader, "lidiffuse", diffuseColor);
	_shader_gpu->SetUniform(FragmentShader, "lispecular", lispecular);


	for (unsigned int i = 0, size = mMeshes.size(); i < size; ++i) 
	{		
		glm::vec3 myColor = mMeshes[i].GetMaterial()->GetColor(MaterialColorDiffuse);

		// material properties
		_shader_gpu->SetUniform(FragmentShader, "matambient", matambient);
		_shader_gpu->SetUniform(FragmentShader, "matdiffuse", myColor);
		_shader_gpu->SetUniform(FragmentShader, "matspecular", matspecular); // specular lighting doesn't have full effect on this object's material
		_shader_gpu->SetUniform(FragmentShader, "matshininess", 32.0f);

		mMeshes[i].Draw();
	}

	//draw test info
	TestHelper::Instance()->AddInfoText("Skinning (GPU) Gltf model test  - color.");
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();
}