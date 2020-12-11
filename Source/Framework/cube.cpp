// Created by Papaya Game Studio

// This includes:
#include "cube.h"

// Local includes:
#include "entity.h"
#include "sprite.h"
#include "texture.h"
#include "SDL.h"

// Library includes:

// Static Members:

Cube::Cube()
: m_activePiece(false)
, m_r(0)
, m_g(0)
, m_b(0)
{
	Entity::Entity();
}

Cube::~Cube()
{
	Entity::~Entity();
}

void
Cube::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);
}

void 
Cube::Process(float deltaTime)
{
	Entity::Process(deltaTime);
}

void 
Cube::Draw(BackBuffer& backBuffer)
{
	SDL_Texture* cubeTexture = m_pSprite->GetTexture()->GetTexture();
	//Reset color and set the saved color for this cube
	SDL_SetTextureColorMod(cubeTexture, 0, 0, 0);
	SDL_SetTextureColorMod(cubeTexture, m_r, m_g, m_b);

	Entity::Draw(backBuffer);
}


void 
Cube::SetActive(bool isActive)
{
	m_activePiece = isActive;
}
bool
Cube::GetActive()
{
	return m_activePiece;
}

void 
Cube::SetColor(const int r, const int g, const int b)
{
	//Set the cubes texture color 
	SDL_Texture* cubeTexture = m_pSprite->GetTexture()->GetTexture();
	SDL_SetTextureColorMod(cubeTexture, r, g, b);

	SaveColor(r, g, b);
}

void
Cube::SaveColor(const int r, const int g, const int b)
{
	//Save color
	m_r = r;
	m_g = g;
	m_b = b;
}

int 
Cube::GetColorR()
{
	return m_r;
}
int 
Cube::GetColorG()
{
	return m_g;
}
int 
Cube::GetColorB()
{
	return m_b;
}