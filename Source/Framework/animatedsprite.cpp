// COMP710 GP 2D Framework 2020

// This includes:
#include "animatedsprite.h"

// Local includes:
#include "texture.h"
#include "backbuffer.h"
#include <assert.h>

AnimatedSprite::AnimatedSprite()
	: m_frameSpeed(0.0f)
	, m_frameWidth(0)
	, m_timeElapsed(0.0f)
	, m_currentFrame(0)
	, m_paused(false)
	, m_loop(true)
	, m_animating(false)
{

}

AnimatedSprite::~AnimatedSprite()
{

}

bool
AnimatedSprite::Initialise(Texture& texture)
{
	m_animating = true;

	Sprite::Initialise(texture);

	m_textureWidth = texture.GetWidth();

	StartAnimating();

	return (true);
}

void
AnimatedSprite::AddFrame(int x)
{
	//Add the x coordinate to the frame coordinate container.
	m_frameCoordinates.push_back(x);
}

void
AnimatedSprite::Process(float deltaTime)
{
	if (m_frameCoordinates.size() <= 0)
	{
		//Set up frames
		assert(m_frameWidth > 0);
		assert(m_width > 0);
		for (int framePos = 0; framePos <= m_textureWidth; framePos = framePos + m_frameWidth)
		{
			AddFrame(framePos);
		}

		m_currentFrame = 0;
	}

	//If not paused...
	if (!m_paused)
	{
		// Count the time elapsed.
		m_timeElapsed += deltaTime;

		// If the time elapsed is greater than the frame speed.
		if (m_timeElapsed > m_frameSpeed)
		{
			// Move to the next frame.
			m_currentFrame++;
			// SS04.10: Reset the time elapsed counter.
			m_timeElapsed = 0;
		}

		// SS04.10: If the current frame is greater than the number 
		//          of frame in this animation...
		if (m_currentFrame + 1 >= (signed int)m_frameCoordinates.size())
		{
			// SS04.10: Reset to the first frame.
			m_currentFrame = 0;
			// SS04.10: Stop the animation if it is not looping...
			if (!m_loop)
			{
				m_paused = true;
			}
		}
	}
	else {
		m_animating = false;
	}
}

void
AnimatedSprite::Draw(BackBuffer& backbuffer)
{
	//Draw the particular frame into the backbuffer.
	//          What is the current frame's x coordinate?
	//          What is the frame width?
	if (!m_paused && m_frameCoordinates.size() > 0 && m_currentFrame < (signed int)m_frameCoordinates.size() - 1)
	{
		backbuffer.DrawAnimatedSprite(m_frameCoordinates[m_currentFrame], m_frameWidth, *this);
	}
}

void
AnimatedSprite::SetFrameSpeed(float f)
{
	m_frameSpeed = f;
}

void
AnimatedSprite::SetFrameWidth(int w)
{
	m_frameWidth = w;
}

void
AnimatedSprite::Pause()
{
	m_paused = !m_paused;
}

bool
AnimatedSprite::IsPaused()
{
	return (m_paused);
}

bool
AnimatedSprite::IsAnimating()
{
	return (m_animating);
}

void
AnimatedSprite::StartAnimating()
{
	m_animating = true;
	m_paused = false;

	m_timeElapsed = 0;
	m_currentFrame = 0;
}

bool
AnimatedSprite::IsLooping()
{
	return (m_loop);
}

void
AnimatedSprite::SetLooping(bool b)
{
	m_loop = b;
}