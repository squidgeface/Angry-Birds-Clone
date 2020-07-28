//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : gameManager.cpp
// Description : gameManager implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "gameManager.h"

CGameManager::CGameManager()
	: m_giStaticProgram(0)
	, m_giTextProgram(0)
	, m_fcounter(0)
	, m_bPaused(false)
	, isCut(false)
	, triPCount(0)
	, linePCount(0)
{
	m_pTime = new CTime();
	m_pInput = new CInput();
	m_pOrthoCamera = new CCamera();
	TestCase1 = new CTextLabel();
	TestCase2 = new CTextLabel();
	TestCase3 = new CTextLabel();
	TestCase4 = new CTextLabel();
	Message1 = new CTextLabel();
	Message2 = new CTextLabel();
	Message3 = new CTextLabel();
	Message4 = new CTextLabel();
	MathEquations = new CMathEx();
	triangle = new CPrefab();
	triangle2 = new CPrefab();
	triangle3 = new CPrefab();
	line = new CPrefab();
}

CGameManager::~CGameManager()
{
	
}

//Initialise Glut window
void CGameManager::InitialiseWindow(int argc, char **argv)
{
	//Setup and create glut controlled window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition((int)Utils::HalfScreenW/2, 0);
	glutInitWindowSize(Utils::ScreenWidth, Utils::ScreenHeight);
	glutCreateWindow("OpenGL First Window");

	//Set up all gl function callbacks based on p hardware
	if (glewInit() != GLEW_OK)
	{
		//if glew setup failed then applicaion will not run graphics correctly
		std::cout << "Glew Initialization Failed. Aborting App." << std::endl;
		system("pause");
	}

	//Sets the clear color when calling glclear()
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//Load shader variables
	m_giStaticProgram = ShaderLoader::CreateProgram("Resources/Shaders/vertexshader.txt", "Resources/Shaders/fragshader.txt");
	
	//set culling on
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	//hide mouse
	//glutSetCursor(GLUT_CURSOR_NONE);

	//Initialise time
	m_pTime->InitiateTime();

	//Set Up Camera from class
	m_pOrthoCamera->SetOrtho();
}

//Initialise Menu items
void CGameManager::InitialiseMenu()
{
	//Test 1 - Larange equation
	Message1->SetLabel("Left Click to set points for a triangle. Press 'T' to clear", "Resources/Fonts/BRLNSR.TTF", vec2(-600.0f, 330.0f), vec3(1.0f, 1.0f, 1.0f), 0.4f);
	Message2->SetLabel("Right Click to set points for a line. press 'L' to clear", "Resources/Fonts/BRLNSR.TTF", vec2(-600.0f, 300.0f), vec3(1.0f, 1.0f, 1.0f), 0.4f);
}

//keyboard input functions
void CGameManager::KeyboardDown(unsigned char key, int x, int y)
{
	m_pInput->KeyboardDown(key, x, y);
}
void CGameManager::KeyboardUp(unsigned char key, int x, int y)
{
	m_pInput->KeyboardUp(key, x, y);
}

//mouse input functions
void CGameManager::MouseClick(int button, int state, int x, int y)
{
	m_pInput->MouseClick(button, state, x, y);
}
void CGameManager::MouseMove(int x, int y)
{
	m_pInput->MouseMove(x, y);
}

//render function
void CGameManager::Render()
{
	//Clear buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//switch for game state
	

		Message1->Render();
		Message2->Render();
		if (triPCount == 3)
		{
			triangle->RenderShapes(m_giStaticProgram);
		}

		if (linePCount == 2)
		{
			line->RenderShapes(m_giStaticProgram);
		}

		if (isCut)
		{
			triangle2->RenderShapes(m_giStaticProgram);
			triangle3->RenderShapes(m_giStaticProgram);
		}



	glutSwapBuffers();
	//glFinish();
}
//update function
void CGameManager::Update()
{
//Update Time components
	m_pTime->UpdateTime();
	
//Get user input into proces input function
	ProcessInput(m_pInput->GetKeyState(), m_pInput->GetMouseState());

//switch for game state

		if (triPCount == 3)
		{
			triangle->UpdateShapes(m_pInput);
		}
		if (linePCount == 2)
		{
			line->UpdateShapes(m_pInput);
		}

		if (linePCount == 2 && triPCount == 3)
		{
			CheckCollision();
		}

		if (isCut)
		{
			triangle2->UpdateShapes(m_pInput);
			triangle3->UpdateShapes(m_pInput);
		}
	
	//update game information
	glutPostRedisplay();
}

//input functions
void CGameManager::ProcessInput(InputState* KeyState, InputState* MouseState)
{
								/// *** MAIN MENU INPUT *** ///
	
		//if mouse click counter is 0 (one click at a time)
		if (m_fcounter == 0.0f)
		{
			if (KeyState['t'] == InputState::INPUT_DOWN || KeyState['T'] == InputState::INPUT_DOWN)
			{
				triPCount = 0;
				isCut = false;
			}

			if (MouseState[0] == InputState::INPUT_DOWN)
			{
				if (triPCount < 3)
				{
					if (triPCount == 0)
					{
						triP1 = vec2(m_pInput->GetMouseX(), m_pInput->GetMouseY());
						triPCount++;
					}
					else if (triPCount == 1)
					{
						triP2 = vec2(m_pInput->GetMouseX(), m_pInput->GetMouseY());
						triPCount++;
					}
					else if (triPCount == 2)
					{
						triP3 = vec2(m_pInput->GetMouseX(), m_pInput->GetMouseY());
						triangle->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP1, triP2, triP3);
						triPCount++;
					}
					
				}
				m_fcounter++;
			}			
		}
		else if (MouseState[0] == InputState::INPUT_UP)
		{
			//reset counter on mouse up
			m_fcounter = 0.0f;
		}

		if (m_fcounter2 == 0)
		{
			if (KeyState['l'] == InputState::INPUT_DOWN || KeyState['L'] == InputState::INPUT_DOWN)
			{
				linePCount = 0;
			}

			if (MouseState[2] == InputState::INPUT_DOWN)
			{
				if (linePCount < 2)
				{
					if (linePCount == 0)
					{
						lineP1 = vec2(m_pInput->GetMouseX(), m_pInput->GetMouseY());
						linePCount++;
					}
					else if (linePCount == 1)
					{
						lineP2 = vec2(m_pInput->GetMouseX(), m_pInput->GetMouseY());
						linePCount++;
						line->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::LINE, vec2(), vec2(), vec2(), lineP1, lineP2);
					}
				}
				m_fcounter2++;
			}		
		}
		else if (MouseState[2] == InputState::INPUT_UP)
		{
			//reset counter on mouse up
			m_fcounter2 = 0.0f;
		}
}

void CGameManager::CheckCollision()
{
	//trip1 as top
	if (MathEquations->LineToLine(triP1, triP2, lineP1, lineP2) && MathEquations->LineToLine(triP1, triP3, lineP1, lineP2))
	{
		isCut = true;
		vec2 interP1 = MathEquations->LineIntercept(triP1, triP2, lineP1, lineP2);
		vec2 interP2 = MathEquations->LineIntercept(triP1, triP3, lineP1, lineP2);

		triangle->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP1, interP1, interP2, vec2(), vec2(), vec3(0.0f, 0.0f, 1.0f));
	
		triangle2->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP2, interP1, interP2, vec2(), vec2(), vec3(0.0f, 1.0f, 0.0f));
	
		triangle3->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP3, triP2, interP2, vec2(), vec2(), vec3(1.0f, 0.0f, 0.0f));
	
	}
	else if (MathEquations->LineToLine(triP2, triP3, lineP1, lineP2) && MathEquations->LineToLine(triP2, triP1, lineP1, lineP2))
	{
		isCut = true;
		vec2 interP1 = MathEquations->LineIntercept(triP2, triP1, lineP1, lineP2);
		vec2 interP2 = MathEquations->LineIntercept(triP2, triP3, lineP1, lineP2);

		triangle->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP2, interP1, interP2, vec2(), vec2(), vec3(0.0f, 0.0f, 1.0f));

		triangle2->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP1, interP2, interP1, vec2(), vec2(), vec3(0.0f, 1.0f, 0.0f));

		triangle3->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP3, triP1, interP2, vec2(), vec2(), vec3(1.0f, 0.0f, 0.0f));

	}
	else if (MathEquations->LineToLine(triP3, triP2, lineP1, lineP2) && MathEquations->LineToLine(triP3, triP1, lineP1, lineP2))
	{
		isCut = true;
		vec2 interP1 = MathEquations->LineIntercept(triP3, triP2, lineP1, lineP2);
		vec2 interP2 = MathEquations->LineIntercept(triP3, triP1, lineP1, lineP2);

		triangle->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP3, interP1, interP2, vec2(), vec2(), vec3(0.0f, 0.0f, 1.0f));

		triangle2->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP1, interP1, interP2, vec2(), vec2(), vec3(0.0f, 1.0f, 0.0f));

		triangle3->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::TRI, triP2, triP1, interP1, vec2(), vec2(), vec3(1.0f, 0.0f, 0.0f));

	}
}