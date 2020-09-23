//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : GameManager.h
// Description : GameManager declaration file
// Author      : alexander jenkins
// Mail        : alexander.jenkins@mds.ac.nz
//

#pragma once

#include "utils.h"

class CGameManager
{

public:
	CGameManager();
	~CGameManager();

	void InitialiseMenu();
	void ClearMenu();
	void InitiliaseLevel1();
	void ClearLevel1();
	void InitiliaseLevel2();
	void ClearLevel2();
	void InitialiseWin();
	void ClearWin();
	void InitialiseLose();
	void ClearLose();
	void InitialiseGameOver();
	void ClearGameOver();
	void Update();
	void CreateObject(b2World* World, float SizeX, float SizeY, float PosX, float PosY, String texPath, BShape _shape, float _scaleX = 1.0f, float _scaleY = 1.0f, b2BodyType _bodyType = b2BodyType::b2_dynamicBody);
	void CreateBird();
	void CreateBird2(float _size);
	void CreateClones();
	void CreateBird3();
	void CreateDestructable(b2World* World, float SizeX, float SizeY, float PosX, float PosY, String texPath, float _scaleX = 1.0f, float _scaleY = 1.0f,BShape _shape = BShape::BOX, b2BodyType _type = b2BodyType::b2_dynamicBody);
	void CreateEnemy(b2World* World, float SizeX, float SizeY, float PosX, float PosY, String texPath, float _scaleX, float _scaleY, BShape _shape);
	void JoinObjects(b2Body* _body1, b2Body* _body2, b2Vec2 _anchorB1, b2Vec2 _anchorB2, BJoint _joint, b2Vec2 _wAnchor1 = b2Vec2(), b2Vec2 _wAnchor2 = b2Vec2());

	//math for the distance between two vectors
	float Distancev2(vec2 _source, vec2 _target)
	{
		return sqrtf((_source.x - _target.x) * (_source.x - _target.x) + (_source.y - _target.y) * (_source.y - _target.y));
	}

protected:
	//windown and world pointers
	RenderWindow* Window = 0;
	b2Vec2* Gravity = 0;
	b2World* World = 0;
	
	//world object vectors
	vector<Sprite*> Sprites;
	vector<b2Body*> Bodies;
	vector<Sprite*> DestSprites;
	vector<b2Body*> DestBodies;	
	vector<Sprite*> EnemySprites;
	vector<b2Body*> EnemyBodies;

	//player object pointers
	Sprite* BirdSprite = 0;
	b2Body* BirdBody = 0;
	vector<Sprite*> CloneSprites;
	vector<b2Body*> CloneBodies;
	//arrow sprite
	Sprite* Arrow = 0;
	//sling sprite
	Sprite* SlingSprite = 0;
	//background sprite
	Sprite* BackgroundSprite = 0;
	//mouse position sprite
	Sprite* MouseSprite = 0;
	//Win screen sprite
	Sprite* WinSprite = 0;
	//instructions text
	Text* InstructionsText = 0;
	//menu text
	Text* RetryButton = 0;
	Text* PlayButton = 0;
	Text* QuitButton = 0;


	//variables
	bool IsPressed = false;
	int EnemySize = 0;
	int MouseInitialX = 0;
	int MouseInitialY = 0;
	int MouseReleaseX = 0;
	int MouseReleaseY = 0;
	bool IsFired = false;
	bool Reset = true;
	GameState InGame = GameState::MENU;
	int BirdCount = 0;
	int BirdsUsed[3] = { 0,0,0 };
	int Level = 1;
	float Timer = 0;
	Clock DeltaClock;
	Time DeltaTime;
};

