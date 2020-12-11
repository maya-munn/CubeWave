// Created by Papaya Game Studio

// This includes:
#include "wavegamemanager.h"

// Local includes:
#include "tetrion.h"
#include "backbuffer.h"
#include "sprite.h"
#include "logmanager.h"
#include "soundmanager.h"
#include "texture.h"

// Library includes:
#include <string>
#include <cmath>
#include <ctime>

// Static Members:


WaveGameManager::WaveGameManager(Sprite* tetroCubeSprite, Sprite* tetrionSprite, BackBuffer& backBuffer, std::vector<Sprite*>* p_spriteList)
: m_score(0)
, m_countdown(0.0f)
, m_level(1)
, m_filledRowsCount(0)
, m_totalFilledRows(0)
, m_currentTetrominoIndex(-1)
, m_nextTetrominoIndex(0)
, m_gameOver(false)
, m_paused(false)
, m_activePiece(false)
, m_fallVelocity(0.1f)
, m_pInstanceTetrion(NULL)
{
	//Seed random
	srand((unsigned int)time(0));

	//Copy the passed in Tetrion and TetroCube sprites
	m_pTetroCubeSprite = tetroCubeSprite;
	m_pTetrionSprite = tetrionSprite;

	//Creating new instance of Tetrion grid, and give it the TetroCube sprite
	m_pInstanceTetrion = new Tetrion(m_pTetroCubeSprite);
	m_pInstanceTetrion->Initialise(m_pTetrionSprite);

	//Populate tetromino count array
	for (int tetromino = 0; tetromino < 7; tetromino++)
	{
		m_tetrominoCount[tetromino] = 0;
	}

	//Create UI sprites and text
	m_pCubeArrowUp = backBuffer.CreateSprite("assets\\CubeKeys\\CubeArrowUp.png");		   p_spriteList->push_back(m_pCubeArrowUp);
	m_pCubeArrowDown = backBuffer.CreateSprite("assets\\CubeKeys\\CubeArrowDown.png");	   p_spriteList->push_back(m_pCubeArrowDown);
	m_pCubeArrowRight = backBuffer.CreateSprite("assets\\CubeKeys\\CubeArrowRight.png");   p_spriteList->push_back(m_pCubeArrowRight);
	m_pCubeArrowLeft = backBuffer.CreateSprite("assets\\CubeKeys\\CubeArrowLeft.png");	   p_spriteList->push_back(m_pCubeArrowLeft);
	m_pCubeA = backBuffer.CreateSprite("assets\\CubeKeys\\CubeA.png");					   p_spriteList->push_back(m_pCubeA);
	m_pCubeD = backBuffer.CreateSprite("assets\\CubeKeys\\CubeD.png");					   p_spriteList->push_back(m_pCubeD);
	m_pCubeQ = backBuffer.CreateSprite("assets\\CubeKeys\\CubeQ.png");					   p_spriteList->push_back(m_pCubeQ);
	m_pCubeE = backBuffer.CreateSprite("assets\\CubeKeys\\CubeE.png");					   p_spriteList->push_back(m_pCubeE);
	m_pCubeEsc = backBuffer.CreateSprite("assets\\CubeKeys\\CubeESC.png");				   p_spriteList->push_back(m_pCubeEsc);
	m_pCubeShift = backBuffer.CreateSprite("assets\\CubeKeys\\CubeShift.png");			   p_spriteList->push_back(m_pCubeShift);
	m_pCubeSpace = backBuffer.CreateSprite("assets\\CubeKeys\\CubeSpace.png");			   p_spriteList->push_back(m_pCubeSpace);

	m_pTetO = backBuffer.CreateSprite("assets\\Tetrominos\\TetO.png");		p_spriteList->push_back(m_pTetO);
	m_pTetI = backBuffer.CreateSprite("assets\\Tetrominos\\TetI.png");		p_spriteList->push_back(m_pTetI);
	m_pTetS = backBuffer.CreateSprite("assets\\Tetrominos\\TetS.png");		p_spriteList->push_back(m_pTetS);
	m_pTetZ = backBuffer.CreateSprite("assets\\Tetrominos\\TetZ.png");		p_spriteList->push_back(m_pTetZ);
	m_pTetJ = backBuffer.CreateSprite("assets\\Tetrominos\\TetJ.png");		p_spriteList->push_back(m_pTetJ);
	m_pTetL = backBuffer.CreateSprite("assets\\Tetrominos\\TetL.png");		p_spriteList->push_back(m_pTetL);
	m_pTetT = backBuffer.CreateSprite("assets\\Tetrominos\\TetT.png");		p_spriteList->push_back(m_pTetT);
	
	m_pCubeWaveLogo = backBuffer.CreateSprite("assets\\CubeWaveLogoSmall.png");			   p_spriteList->push_back(m_pCubeWaveLogo);
}


WaveGameManager::~WaveGameManager()
{
	delete m_pInstanceTetrion;
	m_pInstanceTetrion = 0;
}

void 
WaveGameManager::Process(float deltaTime)
{
	if (!m_gameOver && !m_paused)
	{
		//Decrease countdown time with delta time
		m_countdown = (m_countdown - deltaTime);

		//If countdown has gone below 0, process the next natural game step
		if (m_countdown <= 0.0f)
		{
			//Reset countdown
			m_countdown = (m_fallVelocity * (11 - m_level));

			if (m_activePiece)
			{
				//If falling possible 
				if (m_pInstanceTetrion->CheckIfPieceCanFall())
				{
					//Check if no blocks below is blocking
					//Move it one row down
					m_pInstanceTetrion->FallPiece();
				}
				else
				{
					//Commit piece to board by calling Lock method
					//Set active piece to false
					m_pInstanceTetrion->LockPiece();
					SoundManager::GetInstance().PlayLockSFX();
					m_activePiece = false;

					//Clear any full rows 
					int rowsCleared = m_pInstanceTetrion->CheckForFullRows();
					bool perfectClear = false;
					if (rowsCleared > 0)
					{
						SoundManager::GetInstance().PlayClearedRowSFX();
						perfectClear = m_pInstanceTetrion->CheckForPerfectClear();
					}
					m_filledRowsCount += rowsCleared;
					m_totalFilledRows += rowsCleared;

					//Update score
					int turnScore = 0;
					switch (rowsCleared)
					{
					case 0:
						break;
					case 1:
						turnScore = 40 * m_level;
						break;
					case 2:
						turnScore = 100 * m_level;
						break;
					case 3:
						turnScore = 300 * m_level;
						break;
					default:	//4 or more rows
						turnScore = 1200 * m_level;
						break;
					}
					if (perfectClear)
					{
						turnScore *= 10;
					}
					m_score += turnScore;

					//Level up if user has cleared 10 rows since last level up
					if (m_filledRowsCount >= 10)
					{
						if (m_level < 9)
						{
							m_level++;
							m_filledRowsCount = 0;
						}
					}
				}
			}
			else
			{
				if (m_pInstanceTetrion->CheckIfPieceSpawnPossible())
				{
					//Resetting velocity before spawning piece
					ResetDropVelocity();

					//If a piece can spawn, spawn it and set active piece to true
					if (m_currentTetrominoIndex < 0)
					{
						m_currentTetrominoIndex = (rand() % 7);
					}
					else
					{
						m_currentTetrominoIndex = m_nextTetrominoIndex;
					}
					int randomPieceIndex = (rand() % 7);
					m_nextTetrominoIndex = randomPieceIndex;

					m_pInstanceTetrion->SpawnPiece(m_currentTetrominoIndex);
					m_tetrominoCount[m_currentTetrominoIndex]++;

					m_activePiece = true;
				}
				else  //If spawning is impossible, the game is over
				{
					m_gameOver = true;
				}
			}
		}
	}


	//Finally, Process Tetrion
	m_pInstanceTetrion->Process(deltaTime);
}

void 
WaveGameManager::Draw(BackBuffer& backBuffer)
{
	//Draw Tetrion
	m_pInstanceTetrion->Draw(backBuffer);

	//Draw Score
	backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 20);
	backBuffer.SetTextColor(194, 49, 235);

	char scoreTextBuffer[32];
	sprintf(scoreTextBuffer, "%s", "SCORE   ");
	sprintf(scoreTextBuffer + strlen(scoreTextBuffer), "%08d", m_score);
	backBuffer.DrawText(357, 705, scoreTextBuffer);

	//Draw UI components
	DrawControlMapping(backBuffer);
	DrawLevelAndLineStatistics(backBuffer);
	DrawTetrominoStatistics(backBuffer);
	DrawNextTetromino(backBuffer);

	//Draw pause menu if paused
	if (m_paused)
	{
		DisplayPauseMenu(backBuffer);
	}

	//Draw game over menu if game over
	if (m_gameOver)
	{
		DisplayGameOver(backBuffer);
	}
}

Tetrion*
WaveGameManager::GetTetrion()
{
	return m_pInstanceTetrion;
}


void
WaveGameManager::SoftDropPiece()
{
	m_fallVelocity = 0.02f;
}

void 
WaveGameManager::HardDropPiece()
{
	m_fallVelocity = 0.01f;
}

void 
WaveGameManager::ResetDropVelocity()
{
	m_fallVelocity = 0.1f;
}


void
WaveGameManager::DrawControlMapping(BackBuffer& backBuffer)
{
	//Draw title
	backBuffer.SetTextColor(254, 254, 254);
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 30);
	backBuffer.DrawText(40, 88, "CONTROLS");

	
	for (int row = 0; row < 7; row++)
	{
		//Draw Text
		backBuffer.SetTextColor(0, 0, 0);
		backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 10);

		int textX = 10 + 17;
		int textY = ((row * 50) + 10) + 140;

		switch (row)
		{
		case 0:
			backBuffer.DrawText(textX, textY, "MOVE LEFT");
			m_pCubeArrowLeft->SetX(textX + 200);  m_pCubeArrowLeft->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeArrowLeft);
			m_pCubeA->SetX(textX + 240);  m_pCubeA->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeA);
			//Controller
			backBuffer.SetTextColor(0, 0, 255);
			backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 28);
			backBuffer.DrawText(textX + 150, textY - 10, "X");
			break;
		case 1:
			backBuffer.DrawText(textX, textY, "MOVE RIGHT");
			m_pCubeArrowRight->SetX(textX + 200);  m_pCubeArrowRight->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeArrowRight);
			m_pCubeD->SetX(textX + 240);  m_pCubeD->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeD);
			//Controller
			backBuffer.SetTextColor(255, 0, 0);
			backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 28);
			backBuffer.DrawText(textX + 150, textY - 10, "B");
			break;
		case 2:
			backBuffer.DrawText(textX, textY, "ROTATE LEFT");
			m_pCubeArrowUp->SetX(textX + 200);  m_pCubeArrowUp->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeArrowUp);
			m_pCubeQ->SetX(textX + 240);  m_pCubeQ->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeQ);
			//Controller
			backBuffer.SetTextColor(255, 255, 255);
			backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 24);
			backBuffer.DrawText(textX + 140, textY - 10, "LB");
			break;
		case 3:
			backBuffer.DrawText(textX, textY, "ROTATE RIGHT");
			m_pCubeArrowDown->SetX(textX + 200);  m_pCubeArrowDown->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeArrowDown);
			m_pCubeE->SetX(textX + 240);  m_pCubeE->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeE);
			//Controller
			backBuffer.SetTextColor(255, 255, 255);
			backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 24);
			backBuffer.DrawText(textX + 140, textY - 10, "RB");
			break;
		case 4:
			backBuffer.DrawText(textX, textY, "SOFT DROP");
			m_pCubeSpace->SetX(textX + 220);  m_pCubeSpace->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeSpace);
			//Controller
			backBuffer.SetTextColor(0, 255, 0);
			backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 28);
			backBuffer.DrawText(textX + 150, textY - 10, "A");
			break;
		case 5:
			backBuffer.DrawText(textX, textY, "HARD DROP");
			m_pCubeShift->SetX(textX + 220);  m_pCubeShift->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeShift);
			//Controller
			backBuffer.SetTextColor(245, 221, 66);
			backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 28);
			backBuffer.DrawText(textX + 150, textY - 10, "Y");
			break;
		case 6:
			backBuffer.DrawText(textX, textY, "PAUSE");
			m_pCubeEsc->SetX(textX + 220);  m_pCubeEsc->SetY(textY - 10);
			backBuffer.DrawSprite(*m_pCubeEsc);
			//Controller
			backBuffer.SetTextColor(255, 255, 255);
			backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 10);
			backBuffer.DrawText(textX + 135, textY - 10, "START");
			break;
		}
	}
}

void 
WaveGameManager::DrawLevelAndLineStatistics(BackBuffer& backBuffer)
{
	//Draw current level text
	backBuffer.SetTextColor(254, 254, 254);
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 18);
	backBuffer.DrawText(672, 498, "CURRENT");
	backBuffer.DrawText(688, 523, "LEVEL");

	//Draw line
	backBuffer.SetDrawColour(250, 250, 250);
	backBuffer.DrawLine(820, 495, 820, 670);

	//Draw lines text
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 22);
	backBuffer.DrawText(835, 510, "LINES");

	//Draw statistic number
	backBuffer.SetTextColor(194, 49, 235);
	backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 50);
	char levelBuffer[8]; sprintf(levelBuffer, "%d", m_level);
	backBuffer.DrawText(730, 575, levelBuffer);
	//Draw line number
	char linesBuffer[8]; sprintf(linesBuffer, "%03d", m_totalFilledRows);
	backBuffer.DrawText(830, 575, linesBuffer);
}

void 
WaveGameManager::DrawTetrominoStatistics(BackBuffer& backBuffer)
{
	//Draw title
	backBuffer.SetTextColor(254, 254, 254);
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 30);
	backBuffer.DrawText(740, 90, "STATS");

	//Draw count
	backBuffer.SetTextColor(0, 0, 0);
	backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 18);

	int textX = 800;
	int textY = 145;
	char countTextBuffer[16];

	for (int tetromino = 0; tetromino < 7; tetromino++)
	{
		sprintf(countTextBuffer, "%s", "    ");
		sprintf(countTextBuffer + strlen(countTextBuffer), "%d", m_tetrominoCount[tetromino]);
		backBuffer.DrawText(textX, textY, countTextBuffer);
		textY += 45;
	}
}

void 
WaveGameManager::DrawNextTetromino(BackBuffer& backBuffer)
{
	//Draw next text
	backBuffer.SetTextColor(254, 254, 254);
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 30);
	backBuffer.DrawText(100, 505, "NEXT");

	//Draw next tetromino
	switch (m_nextTetrominoIndex)
	{
	case Tetrominos::O:
		m_pTetO->SetX(135);   m_pTetO->SetY(575);
		m_pTetO->Draw(backBuffer);
		break;
	case Tetrominos::I:
		m_pTetI->SetX(105);   m_pTetI->SetY(595);
		m_pTetI->Draw(backBuffer);
		break;
	case Tetrominos::S:
		m_pTetS->SetX(120);   m_pTetS->SetY(580);
		m_pTetS->Draw(backBuffer);
		break;
	case Tetrominos::Z:
		m_pTetZ->SetX(120);   m_pTetZ->SetY(580);
		m_pTetZ->Draw(backBuffer);
		break;
	case Tetrominos::J:
		m_pTetJ->SetX(130);   m_pTetJ->SetY(560);
		m_pTetJ->Draw(backBuffer);
		break;
	case Tetrominos::L:
		m_pTetL->SetX(130);   m_pTetL->SetY(560);
		m_pTetL->Draw(backBuffer);
		break;
	case Tetrominos::T:
		m_pTetT->SetX(120);   m_pTetT->SetY(580);
		m_pTetT->Draw(backBuffer);
		break;
	}
}

void 
WaveGameManager::PauseToggle()
{
	m_paused = !m_paused;
	m_logoR = (rand() % 100) + 155;
	m_logoG = (rand() % 100) + 155;
	m_logoB = (rand() % 100) + 155;
}

bool
WaveGameManager::GetPaused()
{
	return m_paused;
}

bool
WaveGameManager::GetGameOver()
{
	return m_gameOver;
}

void 
WaveGameManager::DisplayPauseMenu(BackBuffer& backBuffer)
{
	//Alpha blend to make transparent rectangle border
	backBuffer.SetDrawColour(0, 255, 217, 180);
	backBuffer.DrawRectangle(50, 50, 950, 700);
	//Almost opaque rectangle
	backBuffer.SetDrawColour(179, 0, 255, 255);
	backBuffer.DrawRectangle(100, 100, 900, 650);

	//Draw logo and apply random color to it
	SDL_Texture* logoTexture = m_pCubeWaveLogo->GetTexture()->GetTexture();
	SDL_SetTextureColorMod(logoTexture, m_logoR, m_logoG, m_logoB);
	m_pCubeWaveLogo->SetX(310);
	m_pCubeWaveLogo->SetY(120);
	m_pCubeWaveLogo->Draw(backBuffer);

	//Draw pause text
	backBuffer.SetTextColor(255, 255, 255);
	backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 40);
	backBuffer.DrawText(400, 180, "PAUSED");

	//Draw pause text
	backBuffer.SetTextColor(255, 255, 255);
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 25);
	backBuffer.DrawText(250, 250, "XBOX                  PC");
	backBuffer.DrawText(250, 320, "SELECT       ESC                Unpause");
	backBuffer.DrawText(250, 360, "X                              Q                          Quit ");
}

void 
WaveGameManager::DisplayGameOver(BackBuffer& backBuffer)
{
	//Alpha blend to make transparent rectangle border
	backBuffer.SetDrawColour(0, 255, 217, 180);
	backBuffer.DrawRectangle(50, 50, 950, 700);
	//Almost opaque rectangle
	backBuffer.SetDrawColour(179, 0, 255, 255);
	backBuffer.DrawRectangle(100, 100, 900, 650);

	//Draw logo and apply random color to it
	SDL_Texture* logoTexture = m_pCubeWaveLogo->GetTexture()->GetTexture();
	SDL_SetTextureColorMod(logoTexture, m_logoR, m_logoG, m_logoB);
	m_pCubeWaveLogo->SetX(310);
	m_pCubeWaveLogo->SetY(120);
	m_pCubeWaveLogo->Draw(backBuffer);

	//Draw pause text
	backBuffer.SetTextColor(255, 255, 255);
	backBuffer.ChangeFont(BackBuffer::Fonts::BITS, 40);
	backBuffer.DrawText(300, 180, "GAME OVER");

	char scoreTextBuffer[32];
	sprintf(scoreTextBuffer, "%s", "SCORE   ");
	sprintf(scoreTextBuffer + strlen(scoreTextBuffer), "%08d", m_score);
	backBuffer.DrawText(240, 250, scoreTextBuffer);

	backBuffer.SetTextColor(255, 255, 255);
	backBuffer.ChangeFont(BackBuffer::Fonts::BLOCKS, 30);
	backBuffer.DrawText(300, 360, "X or Q - Back to Main Menu ");
}