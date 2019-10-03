#include "TestFreeTypeFont.h"

#include "../TestHelper.h"
#include "../InputHelper.h"


void TestFreeTypeFont::Init()
{
	_renderManager = RenderManager::Instance();
	_shaderManager = ShaderManager::Instance();
	_textureManager = TextureManager::Instance();

	//chars to cache
	const char * cache = "1234567890.";

	_fontShader = _shaderManager->LoadFromFile("font", "Assets/Shaders/font", "Assets/Shaders/font", TextureColor);
	_fontShader2 = _shaderManager->LoadFromFile("font2", "Assets/Shaders/font2", "Assets/Shaders/font2", TextureColor);

    //TexturedFont::CreateDistanceFieldFont(132, 512, 1024, cache, "Assets/Fonts/DroidSerif-Bold.ttf");
    //TexturedFont::CreateDistanceFieldFont(102, 512, 1024, cache, "Assets/Fonts/DroidSerif-Bold.ttf");
    //TexturedFont::CreateDistanceFieldFont(44, 512, 1024, cache, "Assets/Fonts/DroidSerif-Bold.ttf");
    //TexturedFont::CreateDistanceFieldFont(34, 512, 1024, cache, "Assets/Fonts/DroidSerif-Bold.ttf"); 

	//atlas
	_atlas = new TextureAtlas(1024, 1024);

	//font
	_font2 = new TexturedFont(_atlas, 128, "Assets/Fonts/DroidSerif-Bold.ttf");
	_font2->CacheGlyphs(cache);
	_font2->SetShader(_fontShader);


    _atlas->SaveTextur("test_atlas.png");

    //upload texture
    _atlas->CreateTexture();

    //
    _font1 = new TexturedFont(96, "Assets/Fonts/DroidSerif-Bold.ttf");
    _font1->SetShader(_fontShader2);

	_projection = glm::ortho(0.0f, (float)_renderManager->GetWidth(), (float)_renderManager->GetHeight(), 0.0f, -1.0f, 1.0f);

	_renderManager->SetDepth(false);



}

void TestFreeTypeFont::Enter()
{
	InputHelper::Instance()->Update();
}

void TestFreeTypeFont::CleanUp()
{
	delete _font1;
	delete _font2;
	delete _atlas;

	_shaderManager->Remove(_fontShader);
	_shaderManager->Remove(_fontShader2);
}

void TestFreeTypeFont::Pause()
{

}

void TestFreeTypeFont::Resume()
{

}

void TestFreeTypeFont::GamePause()
{

}

void TestFreeTypeFont::GameResume()
{

}

void TestFreeTypeFont::HandleEvents(GameManager* manager)
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

void TestFreeTypeFont::Update(GameManager* manager)
{

}

void TestFreeTypeFont::Draw(GameManager* manager)
{
	//start frame
	_renderManager->StartFrame();

	//clear screen
	_renderManager->ClearScreen();

	_renderManager->SetDepth(false);

	//_font1->AddText("12345", _renderManager->GetWidth() / 2, _renderManager->GetHeight() / 2, glm::vec3(1.0f, 1.0f, 1.0f), FontCenter);
	//_font1->Draw(_projection);

	_font2->AddText("12345", _renderManager->GetWidth() / 2, _renderManager->GetHeight() / 3, glm::vec3(1.0f, 1.0f, 0.0f), FontCenter);
	_font2->Draw(_projection);

	TestHelper::Instance()->AddInfoText("Font rendering test.");
	TestHelper::Instance()->ShowInfoText();

	//end frame
	_renderManager->EndFrame();
}