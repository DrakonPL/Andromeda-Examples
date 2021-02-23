#include <Andromeda/System/GameState.h>
#include <Andromeda/System/GameManager.h>

#include <Andromeda/Graphics/RenderManager.h>
#include <Andromeda/Graphics/Camera3d.h>
#include <Andromeda/Graphics/ShaderManager.h>
#include <Andromeda/Graphics/Shader.h>
#include <Andromeda/Graphics/GraphicsEnums.h>
#include <Andromeda/Graphics/VertexTypes.h>
#include <Andromeda/Graphics/TextureManager.h>
#include <Andromeda/Graphics/Models/ModelObj.h>
#include <Andromeda/System/Timer.h>
#include <Andromeda/Input/KeyboardDevice.h>
#include <Andromeda/Input/InputManager.h>

using namespace Andromeda::System;
using namespace Andromeda::Graphics;
using namespace Andromeda::Input;

//bullet
#include "bullet/LinearMath/btVector3.h"
#include "bullet/LinearMath/btMatrix3x3.h"
#include "bullet/LinearMath/btTransform.h"
#include "bullet/LinearMath/btQuickprof.h"
#include "bullet/LinearMath/btAlignedObjectArray.h"
#include "bullet/btBulletDynamicsCommon.h"

#include "DynamicCharacterController.h"

class BulletCharacterTest : public GameState
{
private:

	//render manager
	RenderManager* _renderManager;

	//shader manager
	ShaderManager* _shaderManager;

	//texture manager
	TextureManager* _textureManager;

	// Shaders
	Shader* _shader;

	//model
	ModelObj* _soccerModel;

	VertexArrayObject* _floorModel;
	Texture* _floorTexture;

	//cam
	Camera3d* _cam;

	//cam settings
	float _cameraHeight;
	float _minCameraDistance;
	float _maxCameraDistance;

	//timer
	Timer* _timer;
	float _dt;

	//transform
	glm::mat4 _projection;

	//input
	InputManager* _inputManager;
	KeyboardDevice* _keyboard;
	GamepadDevice* _gamepad;

	//Bullet physics - Main objects
	btDynamicsWorld* physDynamicsWorld;
	btBroadphaseInterface* physBroadphase;
	btCollisionDispatcher* physDispatcher;
	btConstraintSolver*	physSolver;
	btDefaultCollisionConfiguration* physCollisionConfiguration;

	//Bullet physics - Scene objects
	btCapsuleShape* physBallShape;
	btRigidBody* physBallBody;

	btCollisionShape* physGroundShape;
	btRigidBody* physGroundBody;

	glm::vec3 desiredVelocity;
	glm::vec3 velocity;
	glm::vec3 contactNormal;
	float maxSpeed = 10.0f;
	float maxAcceleration = 10.0f, maxAirAcceleration = 1.0f;

	DynamicCharacterController* controller;

private:

	void InitModels();
	void InitPhysic();

	glm::mat4 btScalar2glmMat4(btScalar* matrix);

public:

	void Init();
	void CleanUp();

	void Pause();
	void Resume();

	void GamePause();
	void GameResume();

	void HandleEvents(GameManager* manager);
	void Update(GameManager* manager);
	void Draw(GameManager* manager);
};