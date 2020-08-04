#pragma once

#include "utils.h"

class CGameManager
{

public:
	void InitialiseWindow();

	void Update();

	void CreateGround(b2World& World, float X, float Y);

	void CreateObject(b2World& World, float SizeX, float SizeY, float PosX, float PosY, String texPath);

	void UpdatePhysicsShapes();

	bool GetWindowOpen();

protected:
	RenderWindow* window;
	
	vector<Sprite*> Sprites;
	vector<b2Body*> Bodies;
	
};

