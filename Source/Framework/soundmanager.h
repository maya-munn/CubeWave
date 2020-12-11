// Created by Papaya Game Studio

#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

// Forward Declarations
#include "fmod.hpp"

class SoundManager
{
	//Member methods
public:
	static SoundManager& GetInstance();
	static void DestroyInstance();

	void Initialise();

	//Play sound methods
	void PlayBGMusic();

	void PlayLockSFX();
	void PlayMoveSFX();
	void PlayRotateSFX();
	void PlayStartWaveGameSFX();
	void PlayClearedRowSFX();

	void ToggleSound();

	FMOD::System* GetFMODSystem();

protected:
private:
	SoundManager();
	~SoundManager();

	//Member data
public:
protected:
	static SoundManager* sm_pInstance;
private:
	//Sounds
	FMOD::System *system;
	FMOD::Sound* m_pBgMusic;
	FMOD::Sound* m_pLockSFX;
	FMOD::Sound* m_pMoveSFX;
	FMOD::Sound* m_pRotateSFX;
	FMOD::Sound* m_pStartWavegameSFX;
	FMOD::Sound* m_pClearedRowSFX;
	FMOD::SoundGroup* m_pSoundGroup;

	bool m_soundOn;
};

#endif // __SOUNDMANAGER_H__