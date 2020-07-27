#include "mathex.h"

CMathEx::CMathEx()
{
}

CMathEx::~CMathEx()
{
}

bool CMathEx::Larange(vec3 _A, vec3 _B, vec3 _C)
{
	vec3 part1 = Cross(_A, Cross(_B, _C));
	vec3 part2 = (Dot(_A, _C) * _B) - (Dot(_A, _B) * _C);

	if (part1 == part2)
	{
		float LHS1 = part1.x;
		float LHS2 = part1.y;
		float LHS3 = part1.z;
		float RHS1 = part2.x;
		float RHS2 = part2.y;
		float RHS3 = part2.z;

		
		
		LarangeLHS = ToString(LHS1) + ", " + ToString(LHS2) + ", " + ToString(LHS3);
		LarangeRHS = ToString(RHS1) + ", " + ToString(RHS2) + ", " + ToString(RHS3);
		return true;
	}
	else
	{
		return false;
	}
	
}

pointCheck CMathEx::PointPlane(vec3 _point, vec3 _planePoint, vec3 _planeNormal)
{
	float distance = Dot(_planeNormal, _planePoint);
	float value = Dot(_planeNormal, _point) - distance;

	if (value == 0)
	{
		return pointCheck::ON;
	}
	else if (value > 0)
	{
		return pointCheck::INFRONT;
	}
	else if (value < 0)
	{
		return pointCheck::BEHIND;
	}

}

bool CMathEx::LinePlane(vec3 _linePoint1, vec3 _linePoint2, vec3 _planePoint, vec3 _planeNormal)
{
	float t = Dot(_planeNormal, (_planePoint - _linePoint1)) / Dot(_planeNormal, (_linePoint2 - _linePoint1));

	if (t > 0 && t < 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CMathEx::TrianglePlane(vec3 _triPoint1, vec3 _triPoint2, vec3 _triPoint3, vec3 _planePoint, vec3 _planeNormal)
{
	if (LinePlane(_triPoint1, _triPoint2, _planePoint, _planeNormal) || LinePlane(_triPoint1, _triPoint3, _planePoint, _planeNormal) || LinePlane(_triPoint2, _triPoint3, _planePoint, _planeNormal))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int orientation(vec2 p, vec2 q, vec2 r)
{

	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear 

	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

bool onSegment(vec2 p, vec2 q, vec2 r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

bool CMathEx::LineToLine(vec2 _line1P1, vec2 _line1P2, vec2 line2P1, vec2 line2P2)
{
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(_line1P1, _line1P2, line2P1);
	int o2 = orientation(_line1P1, _line1P2, line2P2);
	int o3 = orientation(line2P1, line2P2, _line1P1);
	int o4 = orientation(line2P1, line2P2, _line1P2);

	// General case 
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(_line1P1, line2P1, _line1P2)) return true;

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(_line1P1, line2P2, _line1P2)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(line2P1, _line1P1, line2P2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(line2P1, _line1P2, line2P2)) return true;

	return false; // Doesn't fall in any of the above cases 
	
}



vec2 CMathEx::LineIntercept(vec2 line1P1, vec2 line1P2, vec2 line2P1, vec2 line2P2)
{
	// Line AB represented as a1x + b1y = c1 
	float line1y = line1P2.y - line1P1.y;
	float line1x = line1P1.x - line1P2.x;
	float line1C = line1y * (line1P1.x) + line1x * (line1P1.y);

	// Line CD represented as a2x + b2y = c2 
	float line2y = line2P2.y - line2P1.y;
	float line2x = line2P1.x - line2P2.x;
	float line2C = line2y * (line2P1.x) + line2x * (line2P1.y);

	float determinant = line1y * line2x - line2y * line1x;

	float x = (line2x * line1C - line1x * line2C) / determinant;
	float y = (line1y * line2C - line2y * line1C) / determinant;

	return vec2(x, y);
	
}

float CMathEx::Dot(vec3 _A, vec3 _B)
{
	return ((_A.x * _B.x) + (_A.y * _B.y) + (_A.z * _B.z));
}

vec3 CMathEx::Cross(vec3 _A, vec3 _B)
{
	return vec3((_A.y*_B.z - _A.z*_B.y), (_A.z*_B.x - _A.x*_B.z), (_A.x*_B.y - _A.y*_B.x));
}

string CMathEx::GetString(int _in)
{
	if (_in == 0)
	{
		return LarangeLHS;
	}
	else if (_in == 1)
	{
		return LarangeRHS;
	}
	
}

