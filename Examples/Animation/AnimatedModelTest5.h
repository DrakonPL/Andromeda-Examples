#pragma once

#ifndef AnimatedModelTest5_H
#define AnimatedModelTest5_H

#include <Andromeda/System/GameState.h>
#include <Andromeda/System/GameManager.h>

#include <Andromeda/Graphics/RenderManager.h>
#include <Andromeda/Graphics/Camera3d.h>
#include <Andromeda/System/Timer.h>
#include <Andromeda/Input/KeyboardDevice.h>
#include <Andromeda/Input/InputManager.h>

#include <Andromeda/Graphics/Models/AnimatedModel.h>

#include "../Character/OrbitCam.h"

using namespace Andromeda::System;
using namespace Andromeda::Input;
using namespace Andromeda::Graphics;

class AnimatedModelTest5 : public GameState
{
private:

	//render manager
    RenderManager* _renderManager;

	//shader manager
    ShaderManager* _shaderManager;

	//texture manager
	TextureManager* _textureManager;

	// Shaders
    //Shader* _shaderTexture;
    Shader* _shaderColor;
    //Shader* _shaderStatic;

	//texture
	Texture* _texture;

	bool _useMouse;
	bool _firstMouse;
	int moveX;
	int moveY;

	//timer
	Timer* _timer;
	float _dt;

	//transform
	glm::mat4 _projection;

	//input
	InputManager* _inputManager;
	KeyboardDevice* _keyboard;
	GamepadDevice* _gamepad;
	MouseDevice* _mouse;

	AnimatedModel* playerModel;
	AnimatedModel* itemModel;
	std::string _currentAnimation;

	//player data
	glm::vec3 _playerPosition;
	glm::vec2 _playerFacing;
	float _playerRotation;

	float _playerSpeed;

	//floor
	Texture* _floorTexture;
	VertexArrayObject* _floorModel;
	Shader* _floorShader;

	//orbit camera
	OrbitCam* _orbitCam;

	//auto follow
	bool autoFollow;
	float followTimer;

	float targetAngle = 0;
	float camAngle = 0;
	float testAngle = 0;
	float rotateCamAngle = 0;


	//animations
	bool anyAnimation = false;
	float animationTime = 0.0f;

	bool jump = false;
	float jumpTime = 0.0f;

	float turnRefVelocity = 0;

public:

	void Init();
	void Enter();
	void CleanUp();

	void Pause();
	void Resume();

	void GamePause();
	void GameResume();

	void HandleEvents(GameManager* manager);
	void Update(GameManager* manager);
	void Draw(GameManager* manager);
};

#endif
