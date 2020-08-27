//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : main.cpp
// Description : main implementation file
// Author      : alexander jenkins
// Mail        : alexander.jenkins@mds.ac.nz
//

#include "GameManager.h"

int main()
{
	CGameManager* GameManager = new CGameManager();
	
	GameManager->Update();

	return 0;
}
