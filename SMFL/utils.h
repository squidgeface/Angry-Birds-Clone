#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "glm.hpp"

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

//math for magnitude of a vector
float Magnitude(vec2 _source)
{
	return sqrtf((_source.x * _source.x) + (_source.y * _source.y));
}

//math for the distance between two vectors
float Distance(vec2 _source, vec2 _target)
{
	return sqrtf((_source.x - _target.x) * (_source.x - _target.x) + (_source.y - _target.y) * (_source.y - _target.y));
}
