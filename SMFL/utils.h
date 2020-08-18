#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

//Library includes
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;
using namespace sf;
using namespace glm;

const float SCALE = 30.0f;
constexpr auto PI = 3.14159265359;

namespace utils
{
	const float ScreenHeight = 600.0f;
	const float ScreenWidth = 800.0f;
	const float HSHeight = ScreenHeight/2.0f;
	const float HSWidth = ScreenWidth/2.0f;
}

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
};