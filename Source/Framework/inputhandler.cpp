// COMP710 GP 2D Framework 2020

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "logmanager.h"
#include "wavegamemanager.h"
#include "tetrion.h"
#include "soundmanager.h"

// Library includes:
#include <cassert>
#include <SDL.h>

InputHandler::InputHandler()
: m_pGameController(0)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		//SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool 
InputHandler::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);

	if (!m_pGameController)
	{
		LogManager::GetInstance().Log("No controller detected!");
	}

	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	SDL_Event event;

	//Default, universal input settings
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			game.Quit();
		}
		else if (event.type == SDL_KEYDOWN)
		{
			//Empty 
		}

		//Do individual state input handling
		switch (game.GetCurrentGameState())
		{
		case Game::MAIN_MENU:
			ProcessInputMainMenu(game, event);
			break;
		case Game::WAVEGAME:
			ProcessInputGame(game, event);
			break;
		}
	}
	
}

void
InputHandler::ProcessInputMainMenu(Game& game, SDL_Event event)
{
	//Key events
	if (event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN)
	{
		//Start game using 1 key
		if (event.key.keysym.sym == SDLK_1 || event.jbutton.button == SDL_CONTROLLER_BUTTON_A)
		{
			game.ChangeGameState(Game::m_gameState::WAVEGAME);
			SoundManager::GetInstance().PlayStartWaveGameSFX();
		}
		//Quit Game using ESC key
		else if (event.key.keysym.sym == SDLK_ESCAPE || event.jbutton.button == SDL_CONTROLLER_BUTTON_START)
		{
			game.Quit();
		}
		//Open settings with RB or S
		else if (event.key.keysym.sym == SDLK_s || event.jbutton.button == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
		{
			game.ToggleSettingsPage();
		}
		else if (event.key.keysym.sym == SDLK_a || event.jbutton.button == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
		{
			game.ToggleAboutPage();
		}
		//Open about with LB or A
	}

	if ((event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN) && game.GetSettingsPageOpen())
	{
		//Toggle sound with Y key
		if (event.key.keysym.sym == SDLK_y || event.jbutton.button == SDL_CONTROLLER_BUTTON_Y)
		{
			SoundManager::GetInstance().ToggleSound();
		}
	}
}

void 
InputHandler::ProcessInputGame(Game& game, SDL_Event event)
{
	//Key press down events for game play
	if (event.type == SDL_KEYDOWN)
	{
		//Process game input if not paused
		if (!game.GetWaveGameManager()->GetPaused())
		{
			//Tetromino left and right movement and rotation
			if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
			{
				game.GetWaveGameManager()->GetTetrion()->MovePiece(Tetrion::MoveDirection::LEFT);
				SoundManager::GetInstance().PlayMoveSFX();
			}
			else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
			{
				game.GetWaveGameManager()->GetTetrion()->MovePiece(Tetrion::MoveDirection::RIGHT);
				SoundManager::GetInstance().PlayMoveSFX();
			}
			else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_q)
			{
				game.GetWaveGameManager()->GetTetrion()->RotatePiece(Tetrion::MoveDirection::LEFT);
				SoundManager::GetInstance().PlayRotateSFX();
			}
			else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_e)
			{
				game.GetWaveGameManager()->GetTetrion()->RotatePiece(Tetrion::MoveDirection::RIGHT);
				SoundManager::GetInstance().PlayRotateSFX();
			}
			//Tetromino fall speed controls
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				//Soft drop
				game.GetWaveGameManager()->SoftDropPiece();
			}
			else if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
			{
				//Hard drop
				game.GetWaveGameManager()->HardDropPiece();
			}
		}

		
	}
	//Controller mapping
	else if (event.type == SDL_JOYBUTTONDOWN)
	{
		//Process game input if not paused
		if (!game.GetWaveGameManager()->GetPaused())
		{
			//Tetromino left and right movement and rotation
			if (event.jbutton.button == SDL_CONTROLLER_BUTTON_X)
			{
				game.GetWaveGameManager()->GetTetrion()->MovePiece(Tetrion::MoveDirection::LEFT);
				SoundManager::GetInstance().PlayMoveSFX();
			}
			else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_B)
			{
				game.GetWaveGameManager()->GetTetrion()->MovePiece(Tetrion::MoveDirection::RIGHT);
				SoundManager::GetInstance().PlayMoveSFX();
			}
			else if (event.jbutton.button == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
			{
				game.GetWaveGameManager()->GetTetrion()->RotatePiece(Tetrion::MoveDirection::LEFT);
				SoundManager::GetInstance().PlayRotateSFX();
			}
			else if (event.jbutton.button == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
			{
				game.GetWaveGameManager()->GetTetrion()->RotatePiece(Tetrion::MoveDirection::RIGHT);
				SoundManager::GetInstance().PlayRotateSFX();
			}
			//Tetromino fall speed controls
			else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_Y)
			{
				//Soft drop
				game.GetWaveGameManager()->SoftDropPiece();
			}
			else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				//Hard drop
				game.GetWaveGameManager()->HardDropPiece();
			}
		}


	}
	else if (event.type == SDL_KEYUP)
	{
		//Releasing soft drop controls to reset it back to normal
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			game.GetWaveGameManager()->ResetDropVelocity();
		}
	}
	else if (event.type == SDL_JOYBUTTONUP)
	{
		//Releasing soft drop controls to reset it back to normal
		if (event.jbutton.button == SDL_CONTROLLER_BUTTON_Y)
		{
			game.GetWaveGameManager()->ResetDropVelocity();
		}
	}

	//Key press down events for general settings/accessing pause menu
	if (event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN)
	{
		if (event.key.keysym.sym == SDLK_ESCAPE || event.jbutton.button == SDL_CONTROLLER_BUTTON_START)
		{
			//Pause game
			game.GetWaveGameManager()->PauseToggle();
		}
	}

	//Key press down events for settings menu WHILE PAUSED
	if ((event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONUP) && game.GetWaveGameManager()->GetPaused())
	{
		if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_x || event.jbutton.button == SDL_CONTROLLER_BUTTON_X)
		{
			//Quit game
			game.QuitWaveGame();
		}
	}

	//Key press down events for settings menu WHILE GAME OVER
	if (game.GetWaveGameManager() != 0)
	{
		if ((event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONUP) && game.GetWaveGameManager()->GetGameOver())
		{
			if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_x || event.jbutton.button == SDL_CONTROLLER_BUTTON_X)
			{
				//Quit game
				game.QuitWaveGame();
			}
		}
	}
}
