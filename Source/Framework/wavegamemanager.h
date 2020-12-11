// Created by Papaya Game Studio

#ifndef __WAVEGAMEMANAGER_H__
#define __WAVEGAMEMANAGER_H__
#pragma once 

// Forward Declarations
class Tetrion;
class BackBuffer;
class Sprite;

#include <vector>

class WaveGameManager
{
//Member methods
public:
	WaveGameManager(Sprite* tetroCubeSprite, Sprite* tetrionSprite, BackBuffer& backBuffer, std::vector<Sprite*>* p_spriteList);
	~WaveGameManager();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void PauseToggle();
	bool GetPaused();
	bool GetGameOver();

	Tetrion* GetTetrion();

	//Tetromino speed manipulation methods
	void SoftDropPiece();
	void HardDropPiece();
	void ResetDropVelocity();

	//UI draw methods
	void DrawControlMapping(BackBuffer& backBuffer);
	void DrawTetrominoStatistics(BackBuffer& backBuffer);
	void DrawNextTetromino(BackBuffer& backBuffer);
	void DrawLevelAndLineStatistics(BackBuffer& backBuffer);


protected:
private:
	enum Tetrominos
	{
		O, I, S, Z, L, J, T
	};

	void DisplayPauseMenu(BackBuffer& backBuffer);
	void DisplayGameOver(BackBuffer& backBuffer);

//Member data
public:
protected:
private:
	//Game state variables
	bool m_gameOver;
	bool m_paused;

	int m_score;
	float m_countdown;
	int m_level;
	int m_filledRowsCount;
	int m_totalFilledRows;

	int m_tetrominoCount[7];
	int m_currentTetrominoIndex;
	int m_nextTetrominoIndex;

	float m_fallVelocity;

	bool m_activePiece;

	//Tetrion information
	Sprite* m_pTetroCubeSprite;
	Sprite* m_pTetrionSprite;
	Tetrion* m_pInstanceTetrion;

	std::vector<Sprite*> p_spriteList;
	//Controls UI components
	Sprite* m_pCubeArrowUp;
	Sprite* m_pCubeArrowDown;
	Sprite* m_pCubeArrowRight;
	Sprite* m_pCubeArrowLeft;

	Sprite* m_pCubeA;
	Sprite* m_pCubeD;
	Sprite* m_pCubeQ;
	Sprite* m_pCubeE;

	Sprite* m_pCubeEsc;
	Sprite* m_pCubeShift;
	Sprite* m_pCubeSpace;
	//NEXT tetromino sprites
	Sprite* m_pTetO;
	Sprite* m_pTetI;
	Sprite* m_pTetS;
	Sprite* m_pTetZ;
	Sprite* m_pTetJ;
	Sprite* m_pTetL;
	Sprite* m_pTetT;

	Sprite* m_pCubeWaveLogo;
	int m_logoR;
	int m_logoG;
	int m_logoB;
};

#endif // __WAVEGAMEMANAGER_H__