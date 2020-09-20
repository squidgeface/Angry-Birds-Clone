//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : utils.h
// Description : utils declaration file
// Author      : alexander jenkins
// Mail        : alexander.jenkins@mds.ac.nz
//

#pragma once

//dependency library incluides
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

//standard Library includes
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <math.h>
#include <fstream>
#include <sstream>

//namespace activation
using namespace std;
using namespace sf;
using namespace glm;
//global variables
const float SCALE = 30.0f;
constexpr auto PI = 3.14159265359;
//Utility namespace
namespace utils
{
	const int ScreenHeight = 600;
	const int ScreenWidth = 1000;
	const float HSHeight = ScreenHeight/2.0f;
	const float HSWidth = ScreenWidth/2.0f;
}
//enum classes
enum class BShape
{
	BOX,
	CIRCLE,
	TRIANGLE,
};

enum class BJoint
{
	DIST,
	REVO,
	PULLY,
};

enum class GameState
{
	MENU,
	GAME,
	WIN,
	LOSE,
	END,
};