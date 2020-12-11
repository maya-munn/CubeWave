// COMP710 GP 2D Framework 2020
#ifndef __INPUTHANDER_H__
#define __INPUTHANDER_H__

// Library includes:
#include <SDL.h>

// Forward Declarations:
class Game;
class SoundManager;

class InputHandler
{
	//Member Methods:
public:
	InputHandler();
	~InputHandler();

	bool Initialise();
	void ProcessInput(Game& game);

	void ProcessInputMainMenu(Game& game, SDL_Event event);
	void ProcessInputGame(Game& game, SDL_Event event);

protected:

private:
	InputHandler(const InputHandler& inputHandler);
	InputHandler& operator=(const InputHandler& inputHandler);

	//Member Data:
public:

protected:
	SDL_Joystick* m_pGameController;

private:
};

#endif // __INPUTHANDER_H__
