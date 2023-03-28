//---------------------------------------------------------------------------------
// App.cpp
// Implementation of the calls that are exposed via the App namespace.
//---------------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------------
#include <string>
#include "main.h"
#include "app.h"
#include "SimpleSound.h"
#include "SimpleController.h"
#include "SimpleSprite.h"


//---------------------------------------------------------------------------------
// Utils and externals for system info.

namespace App
{	
	void DrawLine(float sx, float sy, float ex, float ey, float r, float g, float b)
	{
#if APP_USE_VIRTUAL_RES		
		APP_VIRTUAL_TO_NATIVE_COORDS(sx, sy);
		APP_VIRTUAL_TO_NATIVE_COORDS(ex, ey);
#endif
		glBegin(GL_LINES);

		glColor3f(r, g, b); // Yellow
		glVertex2f(sx, sy);
		glVertex2f(ex, ey);

		glEnd();
	}
	
	CSimpleSprite *CreateSprite(const char *fileName, int columns, int rows)
	{
		return new CSimpleSprite(fileName, columns, rows);
	}

	CActor* CreateActor(CVec2 position)
	{
		return new CActor(position);
	}

	bool IsKeyPressed(int key)
	{
		return ((GetAsyncKeyState(key) & 0x8000) != 0);
	}

	void GetMousePos(float &x, float &y)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);	// Get the mouse cursor 2D x,y position			
		ScreenToClient(MAIN_WINDOW_HANDLE, &mousePos);
		x = (float)mousePos.x;
		y = (float)mousePos.y;
		x = (x * (2.0f / WINDOW_WIDTH) - 1.0f);
		y = -(y * (2.0f / WINDOW_HEIGHT) - 1.0f);

#if APP_USE_VIRTUAL_RES		
		APP_NATIVE_TO_VIRTUAL_COORDS(x, y);
#endif
	}
	void PlaySound(const char *fileName, bool looping)
	{
		DWORD flags = (looping) ? DSBPLAY_LOOPING : 0;
		CSimpleSound::GetInstance().PlaySound(fileName, flags);
	}
	void StopSound(const char *fileName)
	{
		CSimpleSound::GetInstance().StopSound(fileName);
	}
	bool IsSoundPlaying(const char *fileName)
	{
		return CSimpleSound::GetInstance().IsPlaying(fileName);
	}
	// This prints a string to the screen
	void Print(float x, float y, const char *st, float r, float g, float b, void *font)
	{
#if APP_USE_VIRTUAL_RES		
		APP_VIRTUAL_TO_NATIVE_COORDS(x, y);
#endif		
		// Set location to start printing text
		glColor3f(r, g, b); // Yellow
		glRasterPos2f(x, y);
		int l = (int)strlen(st);
		for (int i = 0; i < l; i++)
		{
			glutBitmapCharacter(font, st[i]); // Print a character on the screen
		}
	}
	const CController &GetController( int pad )
	{
		return CSimpleControllers::GetInstance().GetController(pad);
	}
	bool AABBIntersects(CBoundingBoxComponent* a, CBoundingBoxComponent* b)
	{

		float aScale = a->GetScale();

		CVec2 positionA = a->GetPosition();
		float aMinX = positionA.m_x;
		float aMinY = positionA.m_y;

		//a->GetPosition(aMinX, aMinY);

		aMinX = (aMinX - ((a->GetSize().m_x / 4.0f) * aScale));
		aMinY = (aMinY - ((a->GetSize().m_y / 4.0f) * aScale));
		float aMaxX = (aMinX + ((a->GetSize().m_x / 2.0f) * aScale));
		float aMaxY = (aMinY + ((a->GetSize().m_y / 2.0f) * aScale));


		float bScale = b->GetScale();
		CVec2 positionB = b->GetPosition();
		float bMinX = positionB.m_x;
		float bMinY = positionB.m_y;

		bMinX = (bMinX - ((b->GetSize().m_x / 4.0f) * bScale));
		bMinY = (bMinY - ((b->GetSize().m_y / 4.0f) * bScale));
		float bMaxX = (bMinX + ((b->GetSize().m_x / 2.0f) * bScale));
		float bMaxY = (bMinY + ((b->GetSize().m_y / 2.0f) * bScale));

		return !( aMinX > bMaxX || aMaxX < bMinX || aMinY > bMaxY || aMaxY < bMinY );
	}
	void DrawBoundingBox(CSimpleSprite* sprite)
	{
		float scale = sprite->GetScale();
		float angle = sprite->GetAngle();
		angle = angle * 180 / PI;
		float aMinX;
		float aMinY;
		sprite->GetPosition(aMinX, aMinY);

		aMinX = (aMinX - ((sprite->GetWidth() / 4.0f) * scale));
		aMinY = (aMinY - ((sprite->GetHeight() / 4.0f) * scale));
		float aMaxX = (aMinX + ((sprite->GetWidth() / 2.0f) * scale));
		float aMaxY = (aMinY + ((sprite->GetHeight() / 2.0f) * scale));

		App::DrawLine(aMinX, aMinY, aMaxX, aMinY, 1.0f, 0.0f, 0.0f);
		App::DrawLine(aMinX, aMinY, aMinX, aMaxY, 1.0f, 0.0f, 0.0f);
		App::DrawLine(aMaxX, aMinY, aMaxX, aMaxY, 1.0f, 0.0f, 0.0f);
		App::DrawLine(aMinX, aMaxY, aMaxX, aMaxY, 1.0f, 0.0f, 0.0f);

	}


}