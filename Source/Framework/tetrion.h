// Created by Papaya Game Studio

#ifndef __TETRION_H__
#define __TETRION_H__

// Forward Declarations
class Sprite;
class Entity;
class Vector2;
class BackBuffer;
class Cube;
class Array;

#include "entity.h"
#include <array>

class Tetrion : public Entity
{
	//Member Methods:
public:
	Tetrion(Sprite* tetroCubeSprite);
	~Tetrion();

	void Process(float deltaTime);
	void Initialise(Sprite* tetrionSprite);
	void Draw(BackBuffer& backBuffer);

	int CheckForFullRows(); //Returns the amount of rows removed
	bool CheckIfPieceCanFall();
	bool CheckIfPieceSpawnPossible();
	bool CheckForPerfectClear();

	//Tetrion Translation Methods
	enum MoveDirection
	{
		LEFT, RIGHT, DOWN
	};
	void SpawnPiece(int pieceIndex);
	void LockPiece();
	void RotatePiece(MoveDirection movementDirection);
	void ShiftPiece(MoveDirection moveDirection);
	void FallPiece();
	void MovePiece(MoveDirection moveDirection);

protected:
private:
	//To be called from CheckForFullRows
	void RemoveFullRow(int rowIndex); 

	//Member Data:
public:
	enum Tetrominos
	{
		O, I, S, Z, L, J, T
	};

	

protected:
private:
	//2D Array to hold the positions of each cell in the Tetrion grid
	std::array<std::array<Vector2*, 10>, 22> m_posGrid;	//10x20 grid with 2 non-visible spawn area rows, [row][col]
	//2D Array to hold TetroCubes for each cell
	std::array<std::array<Cube*, 10>, 22> m_cubeGrid;

	Sprite* m_pTetroCubeSprite;

	//Currently active Tetromino cube positions and color
	int m_tetroColourR;
	int m_tetroColourG;
	int m_tetroColourB;

	//Position, rotation state
	int m_tetroCubeGridPositions[4][2]; //[cubeIndex][x or y pos (0 = row, 1 = col)]
	int m_rotationState;

	int m_currentTetromino;
};

#endif // __TETRION_H__
