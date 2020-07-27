//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : prefab.cpp
// Description : prefab implementaion file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "prefab.h"

CPrefab::CPrefab()
	: m_fTexPos(0)
	, m_pCamera(0)
	, m_pInput(0)
	, m_pTime(0)
	, m_fRotationAngle(0)
	, m_eMeshType(MeshType::BLANK)
{
	m_pMesh = new CMesh();
	m_pTexture = new CTexture();
}



CPrefab::~CPrefab()
{
	delete m_pMesh;
	m_pMesh = 0;
	delete m_pTexture;
	m_pTexture = 0;
}

void CPrefab::Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, vec2 tri1,vec2 tri2,vec2 tri3, vec2 line1, vec2 line2, vec3 color)
{

	m_pInput = input;
	m_pTime = timer;
	m_pCamera = camera;
	m_eMeshType = type;
	

	switch (type)
	{
	case MeshType::TRI:
		m_pMesh->SetTriPoints(tri1, tri2, tri3, color);
		break;
	case MeshType::LINE:
		m_pMesh->SetLinePoints(line1, line2);
		break;
	
	default:
		break;
	}

	//position
	m_v3ObjPosition = vec3(0.0f, 0.0f, 0.0f);
	m_m4TranslationMatrix = translate(mat4(), m_v3ObjPosition);

	//Rotation
	m_v3RotationAxisX = vec3(1.0f, 0.0f, 0.0f);
	m_v3RotationAxisY = vec3(0.0f, 1.0f, 0.0f);
	m_v3RotationAxisZ = vec3(0.0f, 0.0f, 1.0f);
	m_fRotationAngle = 0;
	m_m4RotationX = rotate(glm::mat4(), radians(m_fRotationAngle), m_v3RotationAxisX);
	

	//Scale
	m_v3ObjScale = vec3(1.0f, 1.0f, 1.0f);
	m_m4ScaleMatrix = scale(mat4(), m_v3ObjScale);


}

//Initialise Texture as static animated
void CPrefab::InitialiseTextures(const char* data, int slot, bool ani)
{

	m_pTexture->Generate(data, slot);
}
//Render the Shape
void CPrefab::RenderShapes(GLuint program)
{
	//enable blending and alpha channels
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (m_eMeshType == MeshType::CUBE || m_eMeshType == MeshType::SPHERE || m_eMeshType == MeshType::MODEL)
	{
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
	}

	glUseProgram(program);

	//objects local properties and matrix transformation rendering
	GLuint comboLoc = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(comboLoc, 1, GL_FALSE, value_ptr(MVP));
	

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//Texture position
	GLint texPosLoc = glGetUniformLocation(program, "texPos");
	glUniform1f(texPosLoc, m_fTexPos);	

	//Camera Position
	GLint camPosLoc = glGetUniformLocation(program, "camPos");
	glUniform3fv(camPosLoc, 1, glm::value_ptr(m_pCamera->GetCamPos()));
	


	//switch based on mesh type
	switch (m_eMeshType)
	{

	
	case MeshType::TRI:
		//regular render
		m_pTexture->Activate(program, 1);

		//Draw the shapes
		m_pMesh->Draw();
		break;
	
	case MeshType::LINE:
		//regular render
		m_pTexture->Activate(program, 1);

		//Draw the shapes
		m_pMesh->Draw();
		break;
	
	
	default:
		break;
	}

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
}
//Update shape and time variables
void CPrefab::UpdateShapes(CInput* input, CCubemap* _cubeMap, CPrefab* _Object)
{

	


	//Model matrix calculation
	model = m_m4TranslationMatrix * m_m4RotationZ * m_m4RotationX * m_m4RotationY *  m_m4ScaleMatrix;


	//Combo matrix
	MVP = m_pCamera->GetCamera() * m_pCamera->GetView() * model;

}

//math for magnitude of a vector
float CPrefab::Magnitude(vec2 _source)
{
	return sqrtf((_source.x * _source.x) + (_source.y * _source.y));
}
//math for the distance between two vectors
float CPrefab::Distance(vec2 _source, vec2 _target)
{
	return sqrtf((_source.x - _target.x) * (_source.x - _target.x) + (_source.y - _target.y) * (_source.y - _target.y));
}