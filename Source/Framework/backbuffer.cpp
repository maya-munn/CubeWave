// COMP710 GP 2D Framework 2020

// This include:
#include "backbuffer.h"

// Local includes:
#include "logmanager.h"
#include "texturemanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "texture.h"

// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <cassert>

BackBuffer::BackBuffer()
: m_pTextureManager(0)
, m_pWindow(0)
, m_pRenderer(0)
, m_width(0)
, m_height(0)
, m_clearRed(0)
, m_clearGreen(0)
, m_clearBlue(0)
, m_pBlocksFont(0)
, m_pBitFont(0)
, m_textRed(0)
, m_textGreen(0)
, m_textBlue(0)
{

}

BackBuffer::~BackBuffer()
{
	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = 0;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = 0;

	delete m_pTextureManager;
	m_pTextureManager = 0;

	IMG_Quit();
	SDL_Quit();
}

bool 
BackBuffer::Initialise(int width, int height)
{
	m_width = width;
	m_height = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		LogSDLError();
		return (false);
	}
	else
	{
		m_pWindow = SDL_CreateWindow("CUBEWAVE - Tetris Clone by Papaya Games", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	
		if (m_pWindow == 0)
		{
			LogSDLError();
			return (false);
		}
		else
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 

			if (m_pRenderer == 0)
			{
				LogSDLError();
				return (false);
			}
			else
			{
				int imageResult = IMG_Init(IMG_INIT_PNG);

				if (imageResult == 0)
				{
					LogSDLError();
					return (false);
				}
			}
		}
	}

	//Turned fullscreen to OFF
	SetFullscreen(false);
	SDL_Delay(1000);

	//Loading Blocks font
	TTF_Init();
	int fontSize = 30; //10px
	m_pBlocksFont = TTF_OpenFont("assets\\Fonts\\baby-blocks.ttf", fontSize);
	//Loading Bits font
	m_pBitFont = TTF_OpenFont("assets\\Fonts\\8-BIT-WONDER.ttf", fontSize);
	m_pSelectedFont = m_pBlocksFont; //Starting font

	m_pTextureManager = new TextureManager();
	assert(m_pTextureManager);
	m_pTextureManager->Initialise(m_pRenderer);

	return (true);
}

void 
BackBuffer::SetFullscreen(bool fullscreen)
{
	if (fullscreen)
	{
//		SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALWAYS_ON_TOP);
		SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALWAYS_ON_TOP);
		SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
		SDL_SetWindowSize(m_pWindow, m_width, m_height);
	}
	else
	{
		SDL_SetWindowFullscreen(m_pWindow, 0);
	}
}

void 
BackBuffer::Clear()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_clearRed, m_clearGreen, m_clearBlue, 0xff);
	SDL_RenderClear(m_pRenderer);
}

void
BackBuffer::Present()
{
	SDL_RenderPresent(m_pRenderer);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColour(r, g, b, 0xFF);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	//Turn blend mode if alpha has been set to any transparency level
	if (a < 255) 
	{
		SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
	}

	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
}

void 
BackBuffer::DrawSprite(Sprite& sprite)
{
	SDL_Rect dest;

	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = sprite.GetWidth();
	dest.h = sprite.GetHeight();

	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), 0, &dest);
}

void
BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2)
{
	SDL_Rect fillRect;
	
	fillRect.x = x1;
	fillRect.y = y1;
	fillRect.w = x2 - x1;
	fillRect.h = y2 - y1;

	SDL_RenderFillRect(m_pRenderer, &fillRect);
}

void
BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void 
BackBuffer::LogSDLError()
{
	LogManager::GetInstance().Log(SDL_GetError());
}

Sprite* 
BackBuffer::CreateSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	Sprite* pSprite = new Sprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

void 
BackBuffer::SetClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_clearRed = r;
	m_clearGreen = g;
	m_clearBlue = b;
}

// ***** Papaya Custom Methods Below *****
void 
BackBuffer::SetTextColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_textRed = r;
	m_textGreen = g;
	m_textBlue = b;
}

void 
BackBuffer::DrawText(int x, int y, const char* pcText)
{
	SDL_Color color;
	color.r = m_textRed;
	color.g = m_textGreen;
	color.b = m_textBlue;
	color.a = 255; //No transparency

	SDL_Surface* surface = TTF_RenderText_Blended(m_pSelectedFont, pcText, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_pRenderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &surface->w, &surface->h);
	int width = surface->w;
	int height = surface->h;

	SDL_Rect destRect;
	destRect.x = x;
	destRect.y = y;
	destRect.w = width;
	destRect.h = height;

	SDL_RenderCopy(m_pRenderer, texture, NULL, &destRect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void 
BackBuffer::ChangeFont(Fonts font, int size)
{
	TTF_CloseFont(m_pSelectedFont);
	if (font == Fonts::BITS)
	{
		m_pSelectedFont = TTF_OpenFont("assets\\Fonts\\8-BIT-WONDER.ttf", size);
	}
	else if (font == Fonts::BLOCKS)
	{
		m_pSelectedFont = TTF_OpenFont("assets\\Fonts\\baby-blocks.ttf", size);
	}
}

void BackBuffer::DrawAnimatedSprite(int frameXPos, int frameWidth, AnimatedSprite& sprite)
{
	SDL_Rect dest;
	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = frameWidth;
	dest.h = sprite.GetHeight();

	SDL_Rect src;
	src.x = frameXPos;
	src.y = 0;
	src.w = frameWidth;
	src.h = sprite.GetHeight();

	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), &src, &dest);
}

AnimatedSprite*
BackBuffer::CreateAnimatedSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	AnimatedSprite* pSprite = new AnimatedSprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}