// COMP710 GP 2D Framework 2020

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "texture.h"
#include "soundmanager.h"
#include "tetrion.h"
#include "wavegamemanager.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>
#include <cmath>
#include <ctime>

// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, m_splashScreenCount(0)
, m_splashScreenDuration(6) //6 seconds
, m_switchLogoColor(true)
, m_settingsPageOpen(false)
, m_aboutPageOpen(false)
{
	//Seed random time
	srand((unsigned int)time(0));
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_pInputHandler;
	m_pInputHandler = 0;

	// ***** Papaya Custom Code Below *****

	//Delete sprites in sprite list
	int spriteIndex = 0;
	for (Sprite* sprite : m_spriteList)
	{
		delete m_spriteList[spriteIndex];
		m_spriteList[spriteIndex] = 0;
		spriteIndex++;
	}

	//Delete sound manager
	m_pSoundManager->DestroyInstance();
	m_pSoundManager = 0;

	if (m_pWaveGameInstance != 0)
	{
		delete m_pWaveGameInstance;
		m_pWaveGameInstance = 0;
	}

	delete m_pTetrominoAnimated;
	m_pTetrominoAnimated = 0;
	delete m_pTetrominoAnimated2;
	m_pTetrominoAnimated2 = 0;
}

bool 
Game::Initialise()
{
	//Window dimensions - to fit 4:3 aspect ratio
	const int width = 1000;
	const int height = 750;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	m_lastTime = SDL_GetPerformanceCounter();
	m_lag = 0.0f;

	m_pBackBuffer->SetClearColour(0xCC, 0xCC, 0xCC);

	// ***** Papaya Custom Code Below *****
	//Initialise Starting State
	m_currentGameState = m_gameState::MAIN_MENU;

	//Create splash screen sprites
	m_pAUTSplashScreen = m_pBackBuffer->CreateSprite("assets\\SplashScreens\\AUTLogo.png");
	m_pFMODSplashScreen = m_pBackBuffer->CreateSprite("assets\\SplashScreens\\FMODLogo.png");
	m_pPapayaSplashScreen = m_pBackBuffer->CreateSprite("assets\\SplashScreens\\PapayaGamesSplashScreen.png");
	m_pAUTSplashScreen->SetY(20);
	m_pFMODSplashScreen->SetY(240);
	m_spriteList.push_back(m_pAUTSplashScreen);
	m_spriteList.push_back(m_pFMODSplashScreen);
	m_spriteList.push_back(m_pPapayaSplashScreen);

	//Create main menu BG and logo sprites
	m_pCubeWaveLogo = m_pBackBuffer->CreateSprite("assets\\CubeWaveLogo.png");
	m_pMainMenuBackground = m_pBackBuffer->CreateSprite("assets\\MainMenuBackground.png");
	m_spriteList.push_back(m_pCubeWaveLogo);
	m_spriteList.push_back(m_pMainMenuBackground);
	//Set position of logo
	m_pCubeWaveLogo->SetX(215);
	m_pCubeWaveLogo->SetY(135);

	//Wave game BG
	m_pWaveGameBackground = m_pBackBuffer->CreateSprite("assets\\WaveGameBackground.png");
	m_spriteList.push_back(m_pWaveGameBackground);
	//Load tetrion sprite
	m_pTetrionSprite = m_pBackBuffer->CreateSprite("assets\\TetrionBackground.png");
	m_spriteList.push_back(m_pTetrionSprite);
	//Create Tetro cube sprite
	m_pTetroCubeSprite = m_pBackBuffer->CreateSprite("assets\\TetroCube.png");
	m_spriteList.push_back(m_pTetroCubeSprite);

	//Create sound manager
	SoundManager::GetInstance().Initialise();

	//Create animated sprite
	m_pTetrominoAnimated = m_pBackBuffer->CreateAnimatedSprite("assets\\TetrominoAnimated.png");
	m_pTetrominoAnimated->SetFrameWidth(120);
	m_pTetrominoAnimated->SetFrameSpeed(1);
	m_pTetrominoAnimated->SetX(70);
	m_pTetrominoAnimated->SetY(320);

	m_pTetrominoAnimated2 = m_pBackBuffer->CreateAnimatedSprite("assets\\TetrominoAnimated.png");
	m_pTetrominoAnimated2->SetFrameWidth(120);
	m_pTetrominoAnimated2->SetFrameSpeed(1);
	m_pTetrominoAnimated2->SetX(790);
	m_pTetrominoAnimated2->SetY(320);

	m_pWaveGameInstance = 0;
	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);
	
	if (m_looping)
	{
		Uint64 current = SDL_GetPerformanceCounter();

		float deltaTime = (current - m_lastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
		
		m_lastTime = current;

		m_executionTime += deltaTime;

		Process(deltaTime);
	
		m_lag += deltaTime;

		int innerLag = 0;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
			++innerLag;
		}

		// DEBUG STUB:
//		char buffer[64];
//		sprintf(buffer, "%f", deltaTime);
//		LogManager::GetInstance().Log(buffer);

		Draw(*m_pBackBuffer);
	}

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		if (m_currentGameState == m_gameState::SPLASH_SCREEN)
		{
			m_splashScreenCount++;
		}
		m_switchLogoColor = true;
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	//Update FMOD System
	SoundManager::GetInstance().GetFMODSystem()->update();

	// Update the game world simulation:
	// ***** Papaya Custom Code Below *****
	switch (m_currentGameState)
	{
	case m_gameState::SPLASH_SCREEN:
		//Update splash screen duration count
		
		if (m_splashScreenCount < 2)
		{
			//Display AUT Logo
			m_pCurrentSplashScreen = m_pAUTSplashScreen;
		}
		else if (m_splashScreenCount >= 2 && m_splashScreenCount < 4)
		{
			m_pCurrentSplashScreen = m_pFMODSplashScreen;
		}
		else if ((m_splashScreenCount >= 4 && m_splashScreenCount < m_splashScreenDuration))
		{
			m_pCurrentSplashScreen = m_pPapayaSplashScreen;
		}
		else
		{
			this->ChangeGameState(m_gameState::MAIN_MENU);
		}
		break;
	case m_gameState::MAIN_MENU:
		//Main menu processing
		m_pTetrominoAnimated->Process(deltaTime);
		m_pTetrominoAnimated2->Process(deltaTime);
		break;
	case m_gameState::WAVEGAME:
		//Wavemode Game processing - only if it contains an instance
		if (m_pWaveGameInstance != NULL)
		{
			m_pWaveGameInstance->Process(deltaTime);
		}
		break;
	}
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	// ***** Papaya Custom Code Below *****
	switch (m_currentGameState)
	{
	case m_gameState::SPLASH_SCREEN:
		//Draw splash screen
		backBuffer.DrawSprite(*m_pCurrentSplashScreen);
		break;
	case m_gameState::MAIN_MENU:
		//Main menu drawing
		//DRAW Menu UI Stuff
		DrawMainMenuUI(backBuffer);
		m_pTetrominoAnimated->Draw(backBuffer);
		m_pTetrominoAnimated2->Draw(backBuffer);
		if (m_aboutPageOpen)
		{
			DrawAboutPage(backBuffer);
		}
		if (m_settingsPageOpen)
		{
			DrawSettingsPage(backBuffer);
		}
		break;
	case m_gameState::WAVEGAME:
		//Game drawing - only if the instance exists
		if (m_pWaveGameInstance != NULL)
		{
			m_pWaveGameBackground->Draw(backBuffer);
			m_pWaveGameInstance->Draw(backBuffer); //Drawing Tetrion
		}
		break;
	}
	
	
	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}


// ***** Papaya Custom Methods Below *****
void 
Game::ChangeGameState(int gameState)
{
	//Check if gameState index number is in range
	switch (gameState)
	{
	case m_gameState::SPLASH_SCREEN:
		break;
	case m_gameState::MAIN_MENU:
		m_currentGameState = m_gameState::MAIN_MENU;
		break;
	case m_gameState::WAVEGAME:
		//Changing to game state
		m_currentGameState = m_gameState::WAVEGAME;
		//Create new Wavegame instance
		m_pWaveGameInstance = new WaveGameManager(m_pTetroCubeSprite, m_pTetrionSprite, *m_pBackBuffer, &m_spriteList);
		break;
	}
}

int
Game::GetCurrentGameState()
{
	return m_currentGameState;
}

WaveGameManager* 
Game::GetWaveGameManager()
{
	return m_pWaveGameInstance;
}

void 
Game::ColorCycleLogo(Sprite* logo, BackBuffer& backBuffer)
{
	SDL_Texture* logoTexture = logo->GetTexture()->GetTexture();

	if (m_switchLogoColor == true)
	{
		m_switchLogoColor = false;
		m_logoRed = (rand() % 255);
		m_logoGreen = (rand() % 255);
		m_logoBlue = (rand() % 255);
	}
	
	SDL_SetTextureColorMod(logoTexture, m_logoRed, m_logoGreen, m_logoBlue);

	//Set logo size and pos
	logo->Draw(backBuffer);
}

void 
Game::QuitWaveGame()
{
	//Switch game state
	ChangeGameState(Game::m_gameState::MAIN_MENU);

	//Delete wave game instance
	delete m_pWaveGameInstance;
	m_pWaveGameInstance = 0;
}

void 
Game::DrawMainMenuUI(BackBuffer& backBuffer)
{
	//Draw background
	m_pMainMenuBackground->Draw(backBuffer);

	//Draw logo
	ColorCycleLogo(m_pCubeWaveLogo, backBuffer);

	//Draw menu text
	backBuffer.SetTextColor(0, 0, 0);
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 30);
	backBuffer.DrawText(310, 320, "1                Start Wavegame");

	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 25);
	backBuffer.SetTextColor(120, 120, 120);
	backBuffer.DrawText(310, 420, "Playmode   Under Construction!");
	backBuffer.DrawText(310, 520, "Buddymode Under Construction!");

	//About and Settings text
	backBuffer.SetTextColor(0, 0, 0);
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 20);
	backBuffer.DrawText(20, 690, "A or LB   About");
	backBuffer.DrawText(785, 690, "S or RB  Settings");

	//Quit text
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 20);
	backBuffer.SetTextColor(255, 255, 255);
	backBuffer.DrawText(390, 690, "Esc or Select to Quit");
}

void 
Game::ToggleAboutPage()
{
	m_aboutPageOpen = !m_aboutPageOpen;
}

void 
Game::ToggleSettingsPage()
{
	m_settingsPageOpen = !m_settingsPageOpen;
}

bool 
Game::GetSettingsPageOpen()
{
	return m_settingsPageOpen;
}

bool
Game::GetAboutPageOpen()
{
	return m_aboutPageOpen;
}

void 
Game::DrawSettingsPage(BackBuffer& backBuffer)
{
	backBuffer.SetDrawColour(255, 255, 255);
	backBuffer.DrawRectangle(15, 280, 975, 670);

	//Draw settings text
	backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 30);
	backBuffer.SetTextColor(0, 0, 0);
	backBuffer.DrawText(320, 300, "SETTINGS");
	//Draw toggle sound
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 25);
	backBuffer.DrawText(220, 400, "Press Y to toggle sound ON or OFF");
	backBuffer.DrawText(220, 600, "Press S or RB to return to menu");
}

void 
Game::DrawAboutPage(BackBuffer& backBuffer)
{
	backBuffer.SetDrawColour(255, 255, 255);
	backBuffer.DrawRectangle(15, 280, 975, 670);

	//Draw settings text
	backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 30);
	backBuffer.SetTextColor(0, 0, 0);
	backBuffer.DrawText(320, 300, "ABOUT");
	//Draw toggle sound
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 22);
	backBuffer.DrawText(50, 400, "This game was created by Maya Ashizumi Munn for GP individual assignment");
	backBuffer.DrawText(50, 440, "Credit to Steffan Hooper for the GP2D framework");
	backBuffer.DrawText(50, 480, "Middleware used: FMOD, SDL2, C++ STL");

	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 25);
	backBuffer.DrawText(220, 600, "Press A or LB to return to menu");
}