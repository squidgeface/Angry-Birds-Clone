//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : ShaderLoader.h
// Description : shader loader header file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once
#ifndef __SHADERLOADER_H__
#define __SHADERLOADER_H__

// Dependency Includes
#include "Utils.h"

// Library Includes
#include <iostream>

class ShaderLoader
{
	
public:
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);

private:
	ShaderLoader(void);
	~ShaderLoader(void);
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char *filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);
};

#endif //__SHADERLOADER_H__