//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : input.h
// Description : input header file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once
#ifndef __INPUT_H__
#define __INPUT_H__

#include "gameManager.h"
#include "camera.h"

//Prototype classes
class CCamera;


class CInput
{

public:
	CInput();
	~CInput();

	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void MouseClick(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	InputState* GetKeyState();
	InputState* GetMouseState();
	int GetMouseX();
	int GetMouseY();



protected:

	int m_imouseX,  m_imouseY, m_icounter;

	InputState KeyState[255];
	InputState MouseState[3];

};

#endif // !__INPUT_H__