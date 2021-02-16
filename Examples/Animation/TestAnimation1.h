#pragma once

#ifndef TestAnimation1_H
#define TestAnimation1_H

#include <Andromeda/System/GameState.h>
#include <Andromeda/System/GameManager.h>

#include <Andromeda/Graphics/RenderManager.h>
#include <Andromeda/Graphics/Camera3d.h>
#include <Andromeda/System/Timer.h>
#include <Andromeda/Input/KeyboardDevice.h>
#include <Andromeda/Input/InputManager.h>

#include "Andromeda/Graphics/Animation/AnimatedMesh.h"

using namespace Andromeda::System;
using namespace Andromeda::Input;
using namespace Andromeda::Graphics;

class TestAnimation1 : public GameState
{
private:

	//render manager
    RenderManager* _renderManager;

	//shader manager
    ShaderManager* _shaderManager;

	//texture manager
	TextureManager* _textureManager;

	//cam
    Camera3d* _cam;

	// Shaders
    Shader* _shader;

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

	//animation stuff
	std::vector<AnimatedMesh> animatedMeshes;

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
