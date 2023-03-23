//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
CActor *player;
CSimpleSprite * ship;
CSimpleSprite *sheep;
enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	
	player = App::CreateActor(CVec2(400.0f, 400.0f));
	CSimpleSprite* player_sprite = App::CreateSprite(".\\TestData\\rainbow.bmp", 7, 1);
	player->SetSprite(player_sprite);
	//float speed = 1.0f / 15.0f;
	//player->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6});
	//testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	//testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	//testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	player->GetSprite()->SetFrame(2);
	player->GetSprite()->SetScale(1.0f);

	sheep = App::CreateSprite(".\\TestData\\rainbow.bmp", 7, 1);
	sheep->SetFrame(2);
	sheep->SetPosition(100.0f, 200.0f);
	sheep->SetScale(1.0f);

	//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{

	if (App::AABBIntersects(player->GetSprite(), sheep)) {
		sheep->SetFrame(4);
	}
	else {
		sheep->SetFrame(2);
	}
	//------------------------------------------------------------------------
	// Example Sprite Code....

	//testSprite2->Update(deltaTime);
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		//player->SetAnimation(ANIM_BACKWARDS);
		float x, y;
		player->GetSprite()->GetPosition(x, y);
		x += 1.0f;
		player->GetSprite()->SetPosition(x, y);
		
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		//player->SetAnimation(ANIM_BACKWARDS);
		float x, y;
		player->GetSprite()->GetPosition(x, y);
		x -= 1.0f;
		player->GetSprite()->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		//player->SetAnimation(ANIM_BACKWARDS);
		float x, y;
		player->GetSprite()->GetPosition(x, y);
		y += 1.0f;
		player->GetSprite()->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		//player->SetAnimation(ANIM_BACKWARDS);
		float x, y;
		player->GetSprite()->GetPosition(x, y);
		y -= 1.0f;
		player->GetSprite()->SetPosition(x, y);
	}

	sheep->Update(deltaTime);
	player->Update(deltaTime);
	/*if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		player->SetScale(player->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		player->SetScale(player->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		player->SetAngle(player->GetAngle() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		player->SetAngle(player->GetAngle() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		player->SetAnimation(-1);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		player->SetVertex(0, player->GetVertex(0) + 5.0f);
	}*/
	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound(".\\TestData\\Test.wav");
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	

	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	//static float a = 0.0f;
	//float r = 1.0f;
	//float g = 1.0f;
	//float b = 1.0f;
	//a += 0.1f;
	//for (int i = 0; i < 20; i++)
	//{

	//	float sx = 200 + sinf(a + i * 0.1f)*60.0f;
	//	float sy = 200 + cosf(a + i * 0.1f)*60.0f;
	//	float ex = 700 - sinf(a + i * 0.1f)*60.0f;
	//	float ey = 700 - cosf(a + i * 0.1f)*60.0f;
	//	g = (float)i / 20.0f;
	//	b = (float)i / 20.0f;
	//	App::DrawLine(sx, sy, ex, ey,r,g,b);
	//}

	//------------------------------------------------------------------------
	// Bounding Box Debug Draw
	player->Draw();
	App::DrawBoundingBox(player->GetSprite());

	sheep->Draw();
	App::DrawBoundingBox(sheep);


	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	//App::Print(100, 100, "Sample Text");

}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete player;
	delete sheep;
	//delete testSprite2;
	//------------------------------------------------------------------------
}