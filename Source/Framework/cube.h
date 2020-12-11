// Created by Papaya Game Studio

#ifndef __CUBE_H__
#define __CUBE_H__
#pragma once 

// Forward Declarations
class Entity;
class Sprite;
class Texture;

#include "entity.h"

class Cube : public Entity
{
	//Member Methods:
public:
	Cube();
	~Cube();

	void Initialise(Sprite* sprite);
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	//Cube visual methods
	void SetActive(bool isActive);
	bool GetActive();

	void SetColor(const int r, const int g, const int b);
	void SaveColor(const int r, const int g, const int b);

	int GetColorR();
	int GetColorG();
	int GetColorB();

protected:
private:

	//Member Data:
public:
protected:
private:

	bool m_activePiece;

	//Variables to hold cubes color values
	int m_r;
	int m_g;
	int m_b;
};

#endif // __CUBE_H__