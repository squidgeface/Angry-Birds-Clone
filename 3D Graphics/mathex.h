#pragma once

#include "Utils.h"

class CMathEx
{
public:
	CMathEx();
	~CMathEx();

	bool Larange(vec3 _A, vec3 _B, vec3 _C);
	pointCheck PointPlane(vec3 _point, vec3 _plane, vec3 _planeNormal);
	bool LinePlane(vec3 _linePoint1, vec3 _linePoint2, vec3 _planePoint, vec3 _planeNormal);
	bool TrianglePlane(vec3 _triPoint1, vec3 _triPoint2, vec3 _triPoint3, vec3 _planePoint, vec3 _planeNormal);

	bool LineToLine(vec2 _line1P1, vec2 _line1P2, vec2 line2P1, vec2 line2P2);

	vec2 LineIntercept(vec2 line1P1, vec2 line1P2, vec2 line2P1, vec2 line2P2);

	float Dot(vec3 _A, vec3 _B);

	vec3 Cross(vec3 _A, vec3 _B);

	string GetString(int _in);

private:

protected:

	string LarangeLHS;
	string LarangeRHS;

};

