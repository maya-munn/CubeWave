// Created by Papaya Game Studio

// This includes:
#include "soundmanager.h"

// Local includes:


// Library includes:
#include "fmod.hpp"
#include "fmod_errors.h"
#include <SDL.h>
#include <cstdio>

//Static members:
SoundManager* SoundManager::sm_pInstance = 0;

SoundManager::SoundManager()
: m_soundOn(true)
{

}

SoundManager::~SoundManager() 
{
	//Release and close sound
	m_pBgMusic->release();

	m_pSoundGroup->release();
	system->release();
	system->close();
}

SoundManager&
SoundManager::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new SoundManager();
	}

	return *sm_pInstance;
}

void
SoundManager::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

void
SoundManager::Initialise()
{
	//Initialise FMOD Core API - /doc/FMOD API User Manual/white-papers-getting-started.html
	FMOD_RESULT result;
	system = 0;
	result = FMOD::System_Create(&system);      // Create the main system object.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
	result = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	//Initialise sounds
	system->createSoundGroup("Group", &m_pSoundGroup);
	system->createStream("assets\\Sounds\\KorobeinikiPappyRemix.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &m_pBgMusic);	m_pBgMusic->setSoundGroup(m_pSoundGroup);
	system->createSound("assets\\Sounds\\LockTetrominoSFX.wav", FMOD_DEFAULT, 0, &m_pLockSFX);						m_pLockSFX->setSoundGroup(m_pSoundGroup);
	system->createSound("assets\\Sounds\\RotateTetrominoSFX.wav", FMOD_DEFAULT, 0, &m_pRotateSFX);					m_pRotateSFX->setSoundGroup(m_pSoundGroup);
	system->createSound("assets\\Sounds\\MoveTetrominoSFX.wav", FMOD_DEFAULT, 0, &m_pMoveSFX);						m_pMoveSFX->setSoundGroup(m_pSoundGroup);
	system->createSound("assets\\Sounds\\StartWaveGameSFX.wav", FMOD_DEFAULT, 0, &m_pStartWavegameSFX);				m_pStartWavegameSFX->setSoundGroup(m_pSoundGroup);
	system->createSound("assets\\Sounds\\ClearedRowSFX.wav", FMOD_DEFAULT, 0, &m_pClearedRowSFX);					m_pClearedRowSFX->setSoundGroup(m_pSoundGroup);

	//Start playing background music
	PlayBGMusic();
}

FMOD::System* 
SoundManager::GetFMODSystem()
{
	return system;
}


void 
SoundManager::PlayBGMusic()
{
	//Start playing BG music
	if (m_soundOn) {
		system->playSound(m_pBgMusic, 0, false, 0);
	}
}

void 
SoundManager::PlayLockSFX()
{
	if (m_soundOn) {
		system->playSound(m_pLockSFX, 0, false, 0);
	}
}

void 
SoundManager::PlayMoveSFX()
{
	if (m_soundOn) {
		system->playSound(m_pMoveSFX, 0, false, 0);
	}
}

void 
SoundManager::PlayRotateSFX()
{
	if (m_soundOn) {
		system->playSound(m_pRotateSFX, 0, false, 0);
	}
}

void 
SoundManager::PlayStartWaveGameSFX()
{
	if (m_soundOn) {
		system->playSound(m_pStartWavegameSFX, 0, false, 0);
	}
}

void 
SoundManager::PlayClearedRowSFX()
{
	if (m_soundOn) { system->playSound(m_pClearedRowSFX, 0, false, 0); }
}

void 
SoundManager::ToggleSound()
{
	m_soundOn = !m_soundOn;

	if (m_soundOn)
	{
		m_pSoundGroup->setVolume(1.0f);
	}
	else
	{
		m_pSoundGroup->setVolume(0.0f);
	}
}