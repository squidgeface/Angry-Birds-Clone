//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : prefab.h
// Description : prefab header file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//


#ifndef __PREFAB_H__
#define __PREFAB_H__

#include "Utils.h"
#include "mesh.h"
#include "Texture.h"
#include "camera.h"
#include "time.h"
#include "input.h"
#include "cubemap.h"
#include "Model.h"


class CMesh;
class CTexture;
class CCamera;
class CTime;
class CInput;
class CCubemap;
class CModel;

class CPrefab
{

public:

	CPrefab();
	~CPrefab();

	//Game Functions
	void Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, vec2 tri1 = vec2(), vec2 tri2 = vec2(), vec2 tri3 = vec2(), vec2 linep1 = vec2(), vec2 linep2 = vec2(), vec3 color = vec3(1.0f, 0.0f, 0.0f));
	void InitialiseTextures(const char* data, int slot, bool ani = false);
	void RenderShapes(GLuint program);
	void UpdateShapes(CInput* input, CCubemap* cubeMap = NULL, CPrefab* _Object = NULL);

	//math functions
	float Magnitude(vec2 _source);
	float Distance(vec2 _source, vec2 _target);

protected:

	//class pointers
	CMesh* m_pMesh;
	CTexture* m_pTexture;
	CCamera* m_pCamera;
	CTime* m_pTime;
	CInput* m_pInput;
	

	//Matrix manipulation variables
	vec3 m_v3ObjPosition, m_v3RotationAxisX, m_v3RotationAxisY, m_v3RotationAxisZ, m_v3ObjScale;
	mat4 m_m4TranslationMatrix, m_m4RotationX, m_m4RotationZ, m_m4RotationY, m_m4ScaleMatrix, model, MVP;
	float m_fRotationAngle, m_fTexPos;
	
	MeshType m_eMeshType;
	
};

#endif //__PREFAB_H__