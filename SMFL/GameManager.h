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

	float angleBetween(const vec2 v1, const vec2 v2)
	{
		float len1 = sqrt(v1.x * v1.x + v1.y * v1.y);
		float len2 = sqrt(v2.x * v2.x + v2.y * v2.y);

		float dot = v1.x * v2.x + v1.y * v2.y;

		float a = dot / (len1 * len2);

		if (a >= 1.0)
			return 0.0 * 180.0f / PI;
		else if (a <= -1.0)
			return PI * 180.0f / PI;
		else
			return acos(a) * 180.0f / PI; // 0..PI
	}
	
	void UpdatePhysicsShapes();

	bool GetWindowOpen();

protected:
	RenderWindow* window;
	
	vector<Sprite*> Sprites;
	vector<b2Body*> Bodies;

	bool isPressed = false;

};

