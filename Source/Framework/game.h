// COMP710 GP 2D Framework 2020
#ifndef __GAME_H__
#define __GAME_H__

// Forward Declarations
class BackBuffer;
class InputHandler;
class AnimatedSprite;
class Sprite;
class WaveGameManager;
class Texture;
class Tetrion;
class SoundManager;

//Local includes
#include <vector>

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	//Game State FSM Methods
	void ChangeGameState(int gameState);
	int GetCurrentGameState();

	WaveGameManager* GetWaveGameManager();
	void QuitWaveGame();

	//Main menu pages
	void ToggleAboutPage();
	void ToggleSettingsPage();
	bool GetSettingsPageOpen();
	bool GetAboutPageOpen();
	void DrawSettingsPage(BackBuffer& backBuffer);
	void DrawAboutPage(BackBuffer& backBuffer);

protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	void ColorCycleLogo(Sprite* logo, BackBuffer& backBuffer);
	void DrawMainMenuUI(BackBuffer& backBuffer);

	//Member Data:
public:
	enum m_gameState
	{
		SPLASH_SCREEN,
		MAIN_MENU,
		WAVEGAME
	};

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	SoundManager* m_pSoundManager;
	bool m_looping;
	
	int m_currentGameState;

	bool m_settingsPageOpen;
	bool m_aboutPageOpen;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	__int64 m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

	int m_splashScreenDuration;
	int m_splashScreenCount;

	// Sprites
	std::vector<Sprite*> m_spriteList;
	Sprite* m_pCurrentSplashScreen;
	Sprite* m_pAUTSplashScreen;
	Sprite* m_pFMODSplashScreen;
	Sprite* m_pPapayaSplashScreen;

	Sprite* m_pCubeWaveLogo;
	Sprite* m_pMainMenuBackground;

	Sprite* m_pWaveGameBackground;
	Sprite* m_pTetrionSprite;
	Sprite* m_pTetroCubeSprite;

	AnimatedSprite* m_pTetrominoAnimated;
	AnimatedSprite* m_pTetrominoAnimated2;

	//Game instances
	WaveGameManager* m_pWaveGameInstance;

	bool m_switchLogoColor;
	int m_logoRed;
	int m_logoGreen;
	int m_logoBlue;
	
private:

};

#endif // __GAME_H__
