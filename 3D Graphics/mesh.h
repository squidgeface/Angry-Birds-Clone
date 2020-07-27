//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : mesh.h
// Description : mesh header file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once
#ifndef __MESH_H__
#define __MESH_H__

#include "Utils.h"

class CMesh
{
public:
	CMesh();
	~CMesh();

	void SetTriPoints(vec2 point1, vec2 point2, vec2 point3, vec3 color = vec3(1.0f, 0.0f, 0.0f));
	void SetLinePoints(vec2 _point1, vec2 _point2);
	void CreateTri();
	
	void Draw();

	

protected:

	GLuint m_pTexture;
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	GLuint IndiceCount;
	int m_iDrawType;
	float m_fFrames;

	//Vertices for a Quad
	GLfloat triVertices[24]{

		//quad 4 corners
		//position 1			//colour 1			//Texture coords	
		0.0f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		//top left
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		//bottom left
		0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f , 0.0f,		//bottm right
		
	};

	//Indices for the quad
	GLuint triIndicies[3]{
		0, 1, 2,	 //first triangle
	};
	
	
};

#endif //__MESH_H__