//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : gameManager.h
// Description : gameManager header file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once
#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

//local includes
#include "Utils.h"
#include "prefab.h"
#include "input.h"
#include "time.h"
#include "camera.h"
#include "TextLabel.h"
#include "audio.h"
#include "cubemap.h"
#include "mathex.h"


//class prototypes
class CPrefab;
class CInput;
class CTime;
class CCamera;
class CTextLabel;
class CAudio;
class CMathEx;


//enum class
enum class GameState
{
	MENU,
	GAME,
	LOSE,
};

class CGameManager
{
public:

	CGameManager();
	~CGameManager();
	//functions
	void InitialiseWindow(int argc, char **argv);
	void InitialiseMenu();
	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void MouseClick(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	void Render();
	void Update();

	void ProcessInput(InputState* KeyState, InputState* MouseState);

	void CheckCollision();

protected:
	//Variables
	//sound FX
		//sound FX
	Sound* m_pTrackBackground;


	//shader programs
	GLuint m_giStaticProgram, m_giTextProgram;


	CTextLabel* Message1;
	CTextLabel* Message2;
	CTextLabel* Message3;
	CTextLabel* Message4;
	CTextLabel* TestCase1;
	CTextLabel* TestCase2;
	CTextLabel* TestCase3;
	CTextLabel* TestCase4;
	CMathEx* MathEquations;

	CPrefab* triangle;
	CPrefab* triangle2;
	CPrefab* triangle3;
	CPrefab* line;

	vec2 triP1;
	vec2 triP2;
	vec2 triP3;
	vec2 lineP1;
	vec2 lineP2;
	

	//Game management objects
	CInput* m_pInput;
	CTime* m_pTime;
	CCamera* m_pOrthoCamera;


	//game variables
	float m_fcounter, m_fcounter2 = 0, triPCount = 0, linePCount = 0;
	bool m_bPaused, isCut;
};

#endif //__GAMEMANAGER_H__