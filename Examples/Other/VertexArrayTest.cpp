

#include "VertexArrayTest.h"
#include <Andromeda/Graphics/GraphicsEnums.h>
#include <stdlib.h>

#include "../TestHelper.h"
#include "../InputHelper.h"


void VertexArrayTest::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	_counter = 0;
	_maxTests = 500;
}

void VertexArrayTest::Enter()
{

}

void VertexArrayTest::CleanUp()
{

}

void VertexArrayTest::Pause()
{

}

void VertexArrayTest::Resume()
{

}

void VertexArrayTest::GamePause()
{

}

void VertexArrayTest::GameResume()
{

}

void VertexArrayTest::HandleEvents(GameManager* manager)
{
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

void VertexArrayTest::Update(GameManager* manager)
{
	if (_counter < _maxTests)
	{
		VertexArrayObject* _arrayObject = _renderManager->CreateVertexArrayObject(TextureColor, StaticDraw);

		//create vertices
		_arrayObject->CreateVertices(36);

		//get vertices
		TextureColorVertex* _simpleData = static_cast<TextureColorVertex*>(_arrayObject->GetVertices());

		//position
		_simpleData[0].x = -0.5f;	_simpleData[0].y = -0.5f;	_simpleData[0].z = -0.5f;
		_simpleData[1].x = 0.5f;	_simpleData[1].y = -0.5f;	_simpleData[1].z = -0.5f;
		_simpleData[2].x = 0.5f;	_simpleData[2].y = 0.5f;	_simpleData[2].z = -0.5f;
		_simpleData[3].x = 0.5f;	_simpleData[3].y = 0.5f;	_simpleData[3].z = -0.5f;
		_simpleData[4].x = -0.5f;	_simpleData[4].y = 0.5f;	_simpleData[4].z = -0.5f;
		_simpleData[5].x = -0.5f;	_simpleData[5].y = -0.5f;	_simpleData[5].z = -0.5f;

		_simpleData[6].x = -0.5f;	_simpleData[6].y = -0.5f;	_simpleData[6].z = 0.5f;
		_simpleData[7].x = 0.5f;	_simpleData[7].y = -0.5f;	_simpleData[7].z = 0.5f;
		_simpleData[8].x = 0.5f;	_simpleData[8].y = 0.5f;	_simpleData[8].z = 0.5f;
		_simpleData[9].x = 0.5f;	_simpleData[9].y = 0.5f;	_simpleData[9].z = 0.5f;
		_simpleData[10].x = -0.5f;	_simpleData[10].y = 0.5f;	_simpleData[10].z = 0.5f;
		_simpleData[11].x = -0.5f;	_simpleData[11].y = -0.5f;	_simpleData[11].z = 0.5f;

		_simpleData[12].x = -0.5f;	_simpleData[12].y = 0.5f;	_simpleData[12].z = 0.5f;
		_simpleData[13].x = -0.5f;	_simpleData[13].y = 0.5f;	_simpleData[13].z = -0.5f;
		_simpleData[14].x = -0.5f;	_simpleData[14].y = -0.5f;	_simpleData[14].z = -0.5f;
		_simpleData[15].x = -0.5f;	_simpleData[15].y = -0.5f;	_simpleData[15].z = -0.5f;
		_simpleData[16].x = -0.5f;	_simpleData[16].y = -0.5f;	_simpleData[16].z = 0.5f;
		_simpleData[17].x = -0.5f;	_simpleData[17].y = 0.5f;	_simpleData[17].z = 0.5f;

		_simpleData[18].x = 0.5f;	_simpleData[18].y = 0.5f;	_simpleData[18].z = 0.5f;
		_simpleData[19].x = 0.5f;	_simpleData[19].y = 0.5f;	_simpleData[19].z = -0.5f;
		_simpleData[20].x = 0.5f;	_simpleData[20].y = -0.5f;	_simpleData[20].z = -0.5f;
		_simpleData[21].x = 0.5f;	_simpleData[21].y = -0.5f;	_simpleData[21].z = -0.5f;
		_simpleData[22].x = 0.5f;	_simpleData[22].y = -0.5f;	_simpleData[22].z = 0.5f;
		_simpleData[23].x = 0.5f;	_simpleData[23].y = 0.5f;	_simpleData[23].z = 0.5f;

		_simpleData[24].x = -0.5f;	_simpleData[24].y = -0.5f;	_simpleData[24].z = -0.5f;
		_simpleData[25].x = 0.5f;	_simpleData[25].y = -0.5f;	_simpleData[25].z = -0.5f;
		_simpleData[26].x = 0.5f;	_simpleData[26].y = -0.5f;	_simpleData[26].z = 0.5f;
		_simpleData[27].x = 0.5f;	_simpleData[27].y = -0.5f;	_simpleData[27].z = 0.5f;
		_simpleData[28].x = -0.5f;	_simpleData[28].y = -0.5f;	_simpleData[28].z = 0.5f;
		_simpleData[29].x = -0.5f;	_simpleData[29].y = -0.5f;	_simpleData[29].z = -0.5f;

		_simpleData[30].x = -0.5f;	_simpleData[30].y = 0.5f;	_simpleData[30].z = -0.5f;
		_simpleData[31].x = 0.5f;	_simpleData[31].y = 0.5f;	_simpleData[31].z = -0.5f;
		_simpleData[32].x = 0.5f;	_simpleData[32].y = 0.5f;	_simpleData[32].z = 0.5f;
		_simpleData[33].x = 0.5f;	_simpleData[33].y = 0.5f;	_simpleData[33].z = 0.5f;
		_simpleData[34].x = -0.5f;	_simpleData[34].y = 0.5f;	_simpleData[34].z = 0.5f;
		_simpleData[35].x = -0.5f;	_simpleData[35].y = 0.5f;	_simpleData[35].z = -0.5f;

		//color
		for (int i = 0; i < 36; i++)
		{
			_simpleData[i].r = 1.0f;	_simpleData[i].g = 1.0f;	_simpleData[i].b = 1.0f;
		}

		//texture
		_simpleData[0].u = 0.0f;	_simpleData[0].v = 0.0f;
		_simpleData[1].u = 1.0f;	_simpleData[1].v = 0.0f;
		_simpleData[2].u = 1.0f;	_simpleData[2].v = 1.0f;
		_simpleData[3].u = 1.0f;	_simpleData[3].v = 1.0f;
		_simpleData[4].u = 0.0f;	_simpleData[4].v = 1.0f;
		_simpleData[5].u = 0.0f;	_simpleData[5].v = 0.0f;

		_simpleData[6].u = 0.0f;	_simpleData[6].v = 0.0f;
		_simpleData[7].u = 1.0f;	_simpleData[7].v = 0.0f;
		_simpleData[8].u = 1.0f;	_simpleData[8].v = 1.0f;
		_simpleData[9].u = 1.0f;	_simpleData[9].v = 1.0f;
		_simpleData[10].u = 0.0f;	_simpleData[10].v = 1.0f;
		_simpleData[11].u = 0.0f;	_simpleData[11].v = 0.0f;

		_simpleData[12].u = 1.0f;	_simpleData[12].v = 0.0f;
		_simpleData[13].u = 1.0f;	_simpleData[13].v = 1.0f;
		_simpleData[14].u = 0.0f;	_simpleData[14].v = 1.0f;
		_simpleData[15].u = 0.0f;	_simpleData[15].v = 1.0f;
		_simpleData[16].u = 0.0f;	_simpleData[16].v = 0.0f;
		_simpleData[17].u = 1.0f;	_simpleData[17].v = 0.0f;

		_simpleData[18].u = 1.0f;	_simpleData[18].v = 0.0f;
		_simpleData[19].u = 1.0f;	_simpleData[19].v = 1.0f;
		_simpleData[20].u = 0.0f;	_simpleData[20].v = 1.0f;
		_simpleData[21].u = 0.0f;	_simpleData[21].v = 1.0f;
		_simpleData[22].u = 0.0f;	_simpleData[22].v = 0.0f;
		_simpleData[23].u = 1.0f;	_simpleData[23].v = 0.0f;

		_simpleData[24].u = 0.0f;	_simpleData[24].v = 1.0f;
		_simpleData[25].u = 1.0f;	_simpleData[25].v = 1.0f;
		_simpleData[26].u = 1.0f;	_simpleData[26].v = 0.0f;
		_simpleData[27].u = 1.0f;	_simpleData[27].v = 0.0f;
		_simpleData[28].u = 0.0f;	_simpleData[28].v = 0.0f;
		_simpleData[29].u = 0.0f;	_simpleData[29].v = 1.0f;

		_simpleData[30].u = 0.0f;	_simpleData[30].v = 1.0f;
		_simpleData[31].u = 1.0f;	_simpleData[31].v = 1.0f;
		_simpleData[32].u = 1.0f;	_simpleData[32].v = 0.0f;
		_simpleData[33].u = 1.0f;	_simpleData[33].v = 0.0f;
		_simpleData[34].u = 0.0f;	_simpleData[34].v = 0.0f;
		_simpleData[35].u = 0.0f;	_simpleData[35].v = 1.0f;


		//create indices
		_arrayObject->CreateIndices(36);

		//get indices
		unsigned short* _indices = static_cast<unsigned short*>(_arrayObject->GetIndices());

		//set data
		for (int i = 0; i < 36; i++)
		{
			_indices[i] = i;
		}

		//generate buffer object
		_arrayObject->Generate();

		//remove object
		delete _arrayObject;

		_counter++;
	}
}

void VertexArrayTest::Draw(GameManager* manager)
{
	//start frame
	_renderManager->StartFrame();

	//clear screen
	_renderManager->ClearScreen();

	//draw test info
	std::string text = std::string("Test VertexArray : ") + std::to_string(_counter);

	TestHelper::Instance()->AddInfoText(text);
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();
}