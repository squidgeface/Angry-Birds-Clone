//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : mesh.cpp
// Description : mesh implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "mesh.h"

CMesh::CMesh()
	: EBO(0)
	, VBO(0)
	, VAO(0)
	, m_fFrames(1.0f)
	, m_pTexture(0)
	, IndiceCount(0)
	, m_iDrawType(0)
{
}

CMesh::~CMesh()
{
}

//tripoints
void CMesh::SetTriPoints(vec2 point1, vec2 point2, vec2 point3, vec3 color)
{
	//Vertices for a Quad
	GLfloat triVertices[24]{

		//quad 4 corners
		//position 1			//colour 1			//Texture coords	
		point1.x, point1.y, 0.0f,		color.x,color.y,color.z,	0.0f, 1.0f,		//top left
		point2.x, point2.y, 0.0f,		color.x,color.y,color.z,	0.0f, 0.0f,		//bottom left
		point3.x, point3.y, 0.0f,		color.x,color.y,color.z,	1.0f , 0.0f,		//bottm right

	};

	//Indices for the quad
	GLuint triIndicies[3]{
		0, 1, 2,	 //first triangle
	};


	//generate vertex shape
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triIndicies), triIndicies, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)0);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),				//Stride = pos + colour + texture
		(GLvoid*)(6 * sizeof(GLfloat)));	//offset from beginning of Vertex (bypass pos and colour)

	glEnableVertexAttribArray(2);

	IndiceCount = sizeof(triIndicies) / sizeof(GLuint);
	m_iDrawType = GL_TRIANGLES;
}
//linePoints
void CMesh::SetLinePoints(vec2 _point1, vec2 _point2)
{
	//Vertices for a Quad
	GLfloat lineVertices[16]{

		//position 1					//colour 1			//Texture coords	
		_point1.x, _point1.y, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,	
		_point2.x, _point2.y, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,	
	};

	//Indices for the quad
	GLuint lineIndicies[2]{
		0, 1,	 //first triangle
	};


	//generate vertex shape
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndicies), lineIndicies, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)0);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),				//Stride = pos + colour + texture
		(GLvoid*)(6 * sizeof(GLfloat)));	//offset from beginning of Vertex (bypass pos and colour)

	glEnableVertexAttribArray(2);

	IndiceCount = sizeof(lineIndicies) / sizeof(GLuint);
	m_iDrawType = GL_LINES;
}

//draw the mesh
void CMesh::Draw()
{
	glBindVertexArray(VAO); //bind VAO
	glDrawElements(m_iDrawType, IndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); //unbind VAO
}
