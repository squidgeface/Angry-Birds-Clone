#pragma once

#include "utils.h"

class CGameManager
{

public:
	void InitialiseWindow();

	void Update();

	void CreateGround(b2World& World, float X, float Y);

	void CreateObject(b2World& World, float SizeX, float SizeY, float PosX, float PosY, String texPath, float _scaleX = 1.0f, float _scaleY = 1.0f);

	//math for magnitude of a vector
	float Magnitudev2(vec2 _source)
	{
		return sqrtf((_source.x * _source.x) + (_source.y * _source.y));
	}

	//math for the distance between two vectors
	float Distancev2(vec2 _source, vec2 _target)
	{
		return sqrtf((_source.x - _target.x) * (_source.x - _target.x) + (_source.y - _target.y) * (_source.y - _target.y));
	}
	
	void UpdatePhysicsShapes();

	bool GetWindowOpen();

protected:
	RenderWindow* window;
	
	vector<Sprite*> Sprites;
	vector<b2Body*> Bodies;

	bool isPressed = false;
	

};

