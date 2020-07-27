//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : audio.h
// Description : audio header file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once
#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Utils.h"

class CAudio
{
public:
	CAudio();
	~CAudio();
	bool InitialiseSoundSystem();
	void PlaySounds(Sound* holder, bool bLoop = false);
	System* GetAudioSystem();

protected:
	//audio system
	System* m_pAudioSystem;

};

#endif //__AUDIO_H__