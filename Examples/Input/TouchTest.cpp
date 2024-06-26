

#include "TouchTest.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../TestHelper.h"
#include "../InputHelper.h"

void TouchTest::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//atlas
	_atlas = new TextureAtlas(256, 256);

	//font
	_font = new TexturedFont(_atlas, 32, "Assets/Fonts/Lobster-Regular.ttf");

	//load some text
	const char * cache = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	_font->CacheGlyphs(cache);
	
	//upload texture
	_atlas->CreateTexture();

	//load shader
	_shader = _shaderManager->LoadFromFile("font", "Assets/Shaders/font", "Assets/Shaders/font", TextureColor);

	//set text
	_font->SetShader(_shader);

	_projection = glm::ortho(0.0f, (float)_renderManager->GetWidth(), (float)_renderManager->GetHeight(), 0.0f, -1.0f, 1.0f);

	_renderManager->SetDepth(false);

	//input
	_inputManager = InputManager::Instance();

	if (_inputManager->GetKayboardCount() > 0)
		_keyboard = _inputManager->GetKeyboardDevice(0);

	if (_inputManager->GetGamepadCount() > 0)
		_gamepad = _inputManager->GetGamepadDevice(0);

	if (_inputManager->GetTouchDeviceCount() > 0)
	{
		_frontTouch = _inputManager->GetTouchDevice(0);
		_backTouch = _inputManager->GetTouchDevice(1);
	}

}

void TouchTest::Enter()
{

}

void TouchTest::CleanUp()
{
	delete _atlas;
	delete _font;

	_shaderManager->Remove(_shader);
}

void TouchTest::Pause()
{

}

void TouchTest::Resume()
{

}

void TouchTest::GamePause()
{

}

void TouchTest::GameResume()
{

}

void TouchTest::HandleEvents(GameManager* manager)
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



void TouchTest::Update(GameManager* manager)
{
    

}

void TouchTest::Draw(GameManager* manager)
{

	//start frame
	_renderManager->StartFrame();

	//clear screen
	_renderManager->ClearScreen();

	if (_gamepad != 0)
	{
		int lX = _gamepad->LeftAnalogX();
		int lY = _gamepad->LeftAnalogY();

		char buffer[128];
		sprintf(buffer, "Left analog : %d %d", lX,lY);
		_font->AddText(buffer, _renderManager->GetWidth() / 2, 75, FontCenter);

		int rX = _gamepad->RightAnalogX();
		int rY = _gamepad->RightAnalogY();

		char buffer2[128];
		sprintf(buffer2, "Right analog : %d %d", rX, rY);
		_font->AddText(buffer2, _renderManager->GetWidth() / 2, 115, FontCenter);


        bool test_up = _gamepad->KeyUp(Gamepad::Right);
        bool test_down = _gamepad->KeyDown(Gamepad::Right);

        if (test_up)
        {
            _font->AddText("Right up true", _renderManager->GetWidth() / 2, 155, FontCenter);
        }else
        {
            _font->AddText("Right up false", _renderManager->GetWidth() / 2, 155, FontCenter);
        }

        if (test_down)
        {
            _font->AddText("Right down true", _renderManager->GetWidth() / 2, 195, FontCenter);
        }
        else
        {
            _font->AddText("Right down false", _renderManager->GetWidth() / 2, 195, FontCenter);
        }
	}

	if (_frontTouch != 0)
	{
		for (int i = 0; i < _frontTouch->GetTouchCount(); i++)
		{
			char buffer[128];
			sprintf(buffer, "Front touch : %d pos %d %d", i, _frontTouch->GetTouchX(i), _frontTouch->GetTouchY(i));
			_font->AddText(buffer, _renderManager->GetWidth() / 2, (i + 2) * 35, FontCenter);
		}
	}

	if (_backTouch != 0)
	{
		for (int i = 0; i < _backTouch->GetTouchCount(); i++)
		{
			char buffer[128];
			sprintf(buffer, "Back touch : %d pos %d %d", i, _backTouch->GetTouchX(i), _backTouch->GetTouchY(i));
			_font->AddText(buffer, _renderManager->GetWidth() / 2, (i + 7) * 35, FontCenter);
		}
	}	

	_font->Draw(_projection);

	//draw test info
	TestHelper::Instance()->AddInfoText("Vita input test.");
	TestHelper::Instance()->ShowInfoText();

	_renderManager->SetDepth(false);

	//end frame
	_renderManager->EndFrame();
}