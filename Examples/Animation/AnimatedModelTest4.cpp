#include "AnimatedModelTest4.h"

#include "../TestHelper.h"
#include "../InputHelper.h"
#include "Andromeda/Graphics/ShaderManager.h"
#include "Andromeda/Graphics/VertexTypes.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include <Andromeda/Math/Math.h>
#include <glm/gtc/matrix_inverse.hpp>


void AnimatedModelTest4::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//load shader
	//_shaderTexture = _shaderManager->LoadFromFile("skinned_gpu_texture", "Assets/Shaders/skinned_gpu_texture", "Assets/Shaders/lit_texture", NormalTextureWeighJoint);
	_shaderColor = _shaderManager->LoadFromFile("skinned_gpu_color", "Assets/Shaders/skinned_gpu_color", "Assets/Shaders/lit_color", NormalTextureWeighJoint);
	// /_shaderStatic = _shaderManager->LoadFromFile("static_color", "Assets/Shaders/static_color", "Assets/Shaders/lit_color", TextureNormal);


    //load texture
	_texture = _textureManager->LoadFromFile("Assets/Models/Gltf/Nate.png");

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

	//player
	//std::string modelFile = Andromeda::FileSystem::FileManager::Instance()->GetMainDirPath() + "Assets/Models/Gltf/Nate3.gltf";
	std::string modelFile = "Assets/Models/Gltf/Ducky2.glb";
	playerModel = new AnimatedModel();
	playerModel->LoadSkinnedModel(modelFile, SkinningType::GPU);
	playerModel->SetShader(_shaderColor);


	//load sword asset
	std::string swordFile = "Assets/Models/Gltf/Axe.gltf";	
	itemModel = new AnimatedModel();
	itemModel->LoadOnly(swordFile);
	itemModel->SetShader(_shaderColor);

	//attach item model to player model bone
	playerModel->AttachModel(itemModel, "armRight", glm::vec3(-0.383824f, 0.193997f, 0), glm::vec3(-35.0f * Andromeda::Math::Math::Deg2Rad, 0.0, 0));

	_currentAnimation = "Idle";
	playerModel->SetCurrentClip(_currentAnimation);

	_playerPosition = glm::vec3(0.0f,0.0f,0.0f);
	_playerSpeed = 4.0f;

	_orbitCam = new OrbitCam(glm::vec3(0.0f, 2.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//floor
	_floorShader = _shaderManager->LoadFromFile("floorShader", "Assets/Shaders/vertex_color_texture_transform_3d", "Assets/Shaders/vertex_color_texture", TextureColor);
	_floorTexture = _textureManager->LoadFromFile("Assets/Images/wood.png", Andromeda::Graphics::TextureFilerType::LinearFilter, Andromeda::Graphics::TextureColorType::Texture_RGBA, Andromeda::Graphics::TextureWrapType::Repeat, 7);

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
		_simpleData[0].u = 0.0f;	_simpleData[0].v = 50.0f;
		_simpleData[1].u = 50.0f;	_simpleData[1].v = 50.0f;
		_simpleData[2].u = 50.0f;	_simpleData[2].v = 0.0f;
		_simpleData[3].u = 50.0f;	_simpleData[3].v = 0.0f;
		_simpleData[4].u = 0.0f;	_simpleData[4].v = 0.0f;
		_simpleData[5].u = 0.0f;	_simpleData[5].v = 50.0f;

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

	//auto follow option
	autoFollow = true;
	followTimer = 0.0f;
}

void AnimatedModelTest4::Enter()
{

}

void AnimatedModelTest4::CleanUp()
{
	delete _timer;
}

void AnimatedModelTest4::Pause()
{

}

void AnimatedModelTest4::Resume()
{

}

void AnimatedModelTest4::GamePause()
{

}

void AnimatedModelTest4::GameResume()
{

}

void AnimatedModelTest4::HandleEvents(GameManager* manager)
{
	float rutnSmoothTime = 0.1f;

	if (_gamepad != 0)
	{
		//
		anyAnimation = false;


		if (_gamepad->RightAnalogX() > 0.1f || _gamepad->RightAnalogY() > 0.1f || _gamepad->RightAnalogX() < -0.1f || _gamepad->RightAnalogY() < -0.1f)
		{
			//camera rotation
			float rotatex = _gamepad->RightAnalogX() * -1.0f * _playerSpeed;
			float rotatey = (_gamepad->RightAnalogY() * -1.0f) * _playerSpeed;

			_orbitCam->MoveOrbit(rotatex, rotatey, _renderManager->GetWidth(), _renderManager->GetHeight());

			autoFollow = false;
			followTimer = 3.0f;
		}

		//player movement
		if(_gamepad->LeftAnalogX() > 0.1f || _gamepad->LeftAnalogY() > 0.1f || _gamepad->LeftAnalogX() < -0.1f || _gamepad->LeftAnalogY() < -0.1f)
		{
			//get vector from left analog stick - it will point direction when we want to go
			_playerFacing = { _gamepad->LeftAnalogX() * -1.0f,_gamepad->LeftAnalogY() };
			_playerFacing = glm::normalize(_playerFacing);

			//convert it to angle
			targetAngle = atan2f(_playerFacing.x, _playerFacing.y) * Andromeda::Math::Math::Rad2Deg;

			//get vector where camera is facing
			glm::vec2 position2(_orbitCam->GetEye().x, _orbitCam->GetEye().z);
			glm::vec2 pivot2(_orbitCam->GetLookAt().x, _orbitCam->GetLookAt().z);

			glm::vec2 cameraDirection = glm::normalize(  (pivot2 - position2));

			//convert it to angle
			camAngle = atan2f(cameraDirection.x, cameraDirection.y) * Andromeda::Math::Math::Rad2Deg;

			//set correct player rotation with smooth transition
			_playerRotation = Andromeda::Math::Math::SmoothDampAngle(_playerRotation * Andromeda::Math::Math::Rad2Deg, targetAngle + camAngle, turnRefVelocity, rutnSmoothTime, 999.0f, _dt);


			if (_playerRotation > 180.0F)
				_playerRotation -= 360.0F;

			if (_playerRotation < -180.0F)
				_playerRotation += 360.0F;

			_playerRotation = _playerRotation * Andromeda::Math::Math::Deg2Rad;

			//calculate where player will be moving
			glm::vec3 front = glm::quat(glm::vec3(0, targetAngle * Andromeda::Math::Math::Deg2Rad, 0)) * glm::vec3( cameraDirection.x, 0.0f, cameraDirection.y);

			//set position 
		    _playerPosition += front * _playerSpeed * _dt;

			//set animation info
			anyAnimation = true;

			//if we are in jump state then don't play running animation
            if (jump)
            {
				return;
            }

			if (_currentAnimation != "Run")
			{
				_currentAnimation = "Run";
				playerModel->FadeToClip(_currentAnimation, 0.3f);
			}
		}

		if (_gamepad->KeyDown(Gamepad::Cross))
		{
			if (_currentAnimation != "Jump")
			{
				_currentAnimation = "Jump";

				//get jump time
				jumpTime = playerModel->GetClipDuration(_currentAnimation);

				//set jump variable
				jump = true;

				//play animation
				playerModel->PlayOnce(_currentAnimation);
			}

			//set animation info
			anyAnimation = true;
		}

		//idle ma siê pojawiæ tylko wtedy je¿eli nie mamy wciœniêtego jakiegoœ guzika animacji
		//oraz nie leci ¿adna animacja która nie koniecznie potrzebuje guzika np jump


		if(!anyAnimation && !jump)
		{
			if (_currentAnimation != "Idle")
			{
				_currentAnimation = "Idle";
				playerModel->FadeToClip(_currentAnimation, 0.3f);
			}
		}		
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

void AnimatedModelTest4::Update(GameManager* manager)
{
	_dt = _timer->GetDelta();

	//update model
	playerModel->Update(_dt);


	if( jump)
	{
		jumpTime -= _dt;

        if (jumpTime <= 0.0f)
        {
			jump = false;
        }
	}

	//get vector where camera is facing
	glm::vec2 position2(_orbitCam->GetEye().x, _orbitCam->GetEye().z);
	glm::vec2 pivot2(_orbitCam->GetLookAt().x, _orbitCam->GetLookAt().z);

	glm::vec2 cameraDirection = glm::normalize((pivot2 - position2));

	//convert it to angle
	camAngle = atan2f(cameraDirection.x, cameraDirection.y) * Andromeda::Math::Math::Rad2Deg;

    if (!autoFollow)
    {
		followTimer -= _dt;

        if (followTimer <= 0.0f)
        {
			autoFollow = true;
        }
    }

    if (autoFollow)
    {
		rotateCamAngle = Andromeda::Math::Math::MoveTowardsAngle(camAngle, _playerRotation * Andromeda::Math::Math::Rad2Deg, 500 * _dt);

		float test = rotateCamAngle - camAngle;
		_orbitCam->MoveOrbit(test, 0, _renderManager->GetWidth(), _renderManager->GetHeight());
    }
}

void AnimatedModelTest4::Draw(GameManager* manager)
{
	//start frame
	_renderManager->StartFrame();

	//clear screen
	_renderManager->ClearScreen();

    //cam view
	glm::vec3 cameraTargetPosition = glm::vec3(_playerPosition.x, 1.0f, _playerPosition.z);
	glm::mat4 camView = glm::lookAt(_orbitCam->GetEye() + glm::vec3(_playerPosition.x, 0, _playerPosition.z), cameraTargetPosition, _orbitCam->GetUpVector());

	//draw floor
	{
		//use shader
		_floorShader->Bind();

		//use texture
		_renderManager->UseTexture(_floorTexture);

		glm::mat4 model{ 1.0 };
		//glm::mat4 view{ 1.0 };
		glm::mat4 mvp{ 1.0 };

		//get view matrix from camera
		//view = _cam->GetViewMatrix();

		//
		mvp = _projection * camView * model;

		_floorShader->SetUniform(VertexShader, "mvp", mvp);

		_floorModel->Draw();
	}

	glm::mat4 playerLocation{ 1.0f };
	//draw player
	{
		//use texture
		_renderManager->UseTexture(_texture);

		//use shader
		_shaderColor->Bind();

		//glm::mat4 view{ 1.0f };
		glm::mat4 mvp{ 1.0f };
		glm::vec3 lit(1, 1, 1);

		playerLocation = glm::translate(playerLocation, _playerPosition);
		playerLocation = glm::rotate(playerLocation, _playerRotation, glm::vec3(0.0f, 1.0f, 0.0f));

		//get view matrix from camera
		//view = _cam->GetViewMatrix();
		mvp = _projection * camView * playerLocation;

		glm::vec3 viewPosition = _orbitCam->GetEye() + glm::vec3(_playerPosition.x, 0, _playerPosition.z);
		glm::vec3 lampPosition = glm::vec3(0.0f, 5.0f, 5.0f);

		glm::mat4 modelInverse = glm::inverseTranspose(playerLocation);


		_shaderColor->SetUniform(VertexShader, "model", playerLocation);
		_shaderColor->SetUniform(VertexShader, "mvp", mvp);
		//_shaderColor->SetUniform(VertexShader, "modelInverse", modelInverse);

		_shaderColor->SetUniform(FragmentShader, "viewPos", viewPosition);
		_shaderColor->SetUniform(FragmentShader, "light", lampPosition);

		playerModel->Draw();
	}


	//draw sword
	{
		//use shader
		_shaderColor->Bind();

		glm::mat4 model{ 1.0f };
		glm::mat4 mvp{ 1.0f };

		model = playerLocation * model;
		mvp = _projection * camView * model;

		glm::vec3 viewPosition = _orbitCam->GetEye() + glm::vec3(_playerPosition.x, 0, _playerPosition.z);
		glm::vec3 lampPosition = glm::vec3(0.0f, 5.0f, 5.0f);

		_shaderColor->SetUniform(VertexShader, "model", model);
		_shaderColor->SetUniform(VertexShader, "mvp", mvp);

		_shaderColor->SetUniform(FragmentShader, "viewPos", viewPosition);
		_shaderColor->SetUniform(FragmentShader, "light", lampPosition);

		itemModel->Draw();
	}

	//draw test info
	TestHelper::Instance()->AddInfoText("Ducky test");
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();
}