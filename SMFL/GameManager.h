#pragma once

#include "utils.h"

class CGameManager
{

public:
	void InitialiseWindow();

	void Update();

	void CreateGround(b2World& World, float X, float Y);
	void CreateBox(b2World& World, int MouseX, int MouseY, float Size); // Spawns a box at MouseX, MouseY

	bool GetWindowOpen();

protected:
	RenderWindow* window;
	
	
};

