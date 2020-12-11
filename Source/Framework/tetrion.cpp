// Created by Papaya Game Studio

// This includes:
#include "tetrion.h"

// Local includes:
#include "sprite.h"
#include "entity.h"
#include "vector2.h"
#include "backbuffer.h"
#include "cube.h"

// Library includes:
#include <array>
#include <cmath>
#include <ctime>

// Static Members:

Tetrion::Tetrion(Sprite* tetroCubeSprite)
: m_pTetroCubeSprite(0)
, m_currentTetromino(0)
, m_rotationState(1)
{
	Entity::Entity();

	m_pTetroCubeSprite = tetroCubeSprite;

	//Seeding random
	srand((unsigned int)time(0));

}

Tetrion::~Tetrion() 
{
	//Delete all Vector2 objects for the grid AND cube objects
	for (int row = 0; row < 22; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			delete m_posGrid.at(row).at(col);
			delete m_cubeGrid.at(row).at(col);
		}
	}
}

void
Tetrion::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	//Initialise GRID POS array
	//Grid starts at 350x75y, so display background at that point
	float gridStartX = 350.0;
	float gridStartY = 75.0 - 60; // (-60 for the two invisible rows)
	int cubeSize = 30;

	this->SetPositionX(gridStartX);
	this->SetPositionY(gridStartY + 60); //Add 60px back so the grid background can be in place

	//300px x 600px grid - makes each grid square 30px width and height
	for (int row = 0; row < 22; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			//Store Vector2 containing the upper left corner of that grid cell
			m_posGrid.at(row).at(col) = new Vector2(gridStartX, gridStartY);

			gridStartX += cubeSize;
		}

		gridStartX = 350;
		gridStartY += cubeSize;
	}

	//Initialise GRID CUBES array
	for (int row = 0; row < 22; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			//Create new Cube and give it the cube sprite
			m_cubeGrid.at(row).at(col) = new Cube();
			m_cubeGrid.at(row).at(col)->Initialise(m_pTetroCubeSprite);

			//Set cubes position relative to the grid positions from pos array
			m_cubeGrid.at(row).at(col)->SetPositionX(m_posGrid.at(row).at(col)->x);
			m_cubeGrid.at(row).at(col)->SetPositionY(m_posGrid.at(row).at(col)->y);
		}
	}
}

void 
Tetrion::Process(float deltaTime)
{
	//Process entity
	Entity::Process(deltaTime);
}

void Tetrion::Draw(BackBuffer& backBuffer)
{
	Entity::Draw(backBuffer);

	//Drawing rectangles on the grid to show players the visualisation of the grid
	for (int row = 0; row < 22; row++)
	{
		if (row >= 2) //Do not draw the first 2 rows (buffer/spawn rows not visible)
		{
			for (int col = 0; col < 10; col++)
			{
				//Store Vector2 containing the upper left corner of that grid cell
				Vector2* currentPos = m_posGrid.at(row).at(col);
				//Purple color
				backBuffer.SetDrawColour(191, 62, 238, 100);
				backBuffer.DrawRectangle((unsigned int)currentPos->x + 1, (unsigned int)currentPos->y + 1, (unsigned int)currentPos->x + 29, (unsigned int)currentPos->y + 29);
			}
		}
	}

	//Draw grid cubes
	for (int row = 0; row < 22; row++)
	{
		if (row >= 2) //Do not draw the first 2 rows (buffer/spawn rows not visible)
		{
			for (int col = 0; col < 10; col++)
			{
				//Only draw the cube if its active
				if (m_cubeGrid.at(row).at(col)->GetActive())
				{
					m_cubeGrid.at(row).at(col)->Draw(backBuffer);
				}
			}
		}
	}
}

//Tetrion Manipulation Methods
void
Tetrion::SpawnPiece(int pieceIndex)
{
	//Save piece type
	m_currentTetromino = pieceIndex;

	//Reset rotation state
	m_rotationState = 1;

	//Set color and position for new piece
	switch (pieceIndex)
	{
	case Tetrominos::O:
		m_tetroColourR = 139; //Purple
		m_tetroColourG = 27;
		m_tetroColourB = 162;

		m_tetroCubeGridPositions[0][0] = 1; m_tetroCubeGridPositions[0][1] = 5;
		m_tetroCubeGridPositions[1][0] = 1; m_tetroCubeGridPositions[1][1] = 4;
		m_tetroCubeGridPositions[2][0] = 0; m_tetroCubeGridPositions[2][1] = 5;
		m_tetroCubeGridPositions[3][0] = 0; m_tetroCubeGridPositions[3][1] = 4;
		break;
	case Tetrominos::I:
		m_rotationState = 4;

		m_tetroColourR = 20; //Blue
		m_tetroColourG = 22;
		m_tetroColourB = 122;

		m_tetroCubeGridPositions[0][0] = 1; m_tetroCubeGridPositions[0][1] = 6;
		m_tetroCubeGridPositions[1][0] = 1; m_tetroCubeGridPositions[1][1] = 5;
		m_tetroCubeGridPositions[2][0] = 1; m_tetroCubeGridPositions[2][1] = 4;
		m_tetroCubeGridPositions[3][0] = 1; m_tetroCubeGridPositions[3][1] = 3;
		break;
	case Tetrominos::S:
		m_tetroColourR = 43; //Light blue
		m_tetroColourG = 153;
		m_tetroColourB = 250;

		m_tetroCubeGridPositions[0][0] = 1; m_tetroCubeGridPositions[0][1] = 4;
		m_tetroCubeGridPositions[1][0] = 1; m_tetroCubeGridPositions[1][1] = 5;
		m_tetroCubeGridPositions[2][0] = 0; m_tetroCubeGridPositions[2][1] = 5;
		m_tetroCubeGridPositions[3][0] = 0; m_tetroCubeGridPositions[3][1] = 6;
		break;
	case Tetrominos::Z:
		m_tetroColourR = 184; //Hot pink
		m_tetroColourG = 16;
		m_tetroColourB = 86;

		m_tetroCubeGridPositions[0][0] = 1; m_tetroCubeGridPositions[0][1] = 6;
		m_tetroCubeGridPositions[1][0] = 1; m_tetroCubeGridPositions[1][1] = 5;
		m_tetroCubeGridPositions[2][0] = 0; m_tetroCubeGridPositions[2][1] = 5;
		m_tetroCubeGridPositions[3][0] = 0; m_tetroCubeGridPositions[3][1] = 4;
		break;
	case Tetrominos::L:
		m_rotationState = 2;

		m_tetroColourR = 253; //Orange
		m_tetroColourG = 142;
		m_tetroColourB = 45;

		m_tetroCubeGridPositions[0][0] = 1; m_tetroCubeGridPositions[0][1] = 4;
		m_tetroCubeGridPositions[1][0] = 0; m_tetroCubeGridPositions[1][1] = 4;
		m_tetroCubeGridPositions[2][0] = 0; m_tetroCubeGridPositions[2][1] = 5;
		m_tetroCubeGridPositions[3][0] = 0; m_tetroCubeGridPositions[3][1] = 6;
		break;
	case Tetrominos::J:
		m_rotationState = 4;

		m_tetroColourR = 194; //Red
		m_tetroColourG = 28;
		m_tetroColourB = 20;

		m_tetroCubeGridPositions[0][0] = 1; m_tetroCubeGridPositions[0][1] = 6;
		m_tetroCubeGridPositions[1][0] = 0; m_tetroCubeGridPositions[1][1] = 6;
		m_tetroCubeGridPositions[2][0] = 0; m_tetroCubeGridPositions[2][1] = 5;
		m_tetroCubeGridPositions[3][0] = 0; m_tetroCubeGridPositions[3][1] = 4;
		break;
	case Tetrominos::T:
		m_tetroColourR = 238; //Yellow
		m_tetroColourG = 255;
		m_tetroColourB = 0;

		m_tetroCubeGridPositions[0][0] = 1; m_tetroCubeGridPositions[0][1] = 5;
		m_tetroCubeGridPositions[1][0] = 0; m_tetroCubeGridPositions[1][1] = 4;
		m_tetroCubeGridPositions[2][0] = 0; m_tetroCubeGridPositions[2][1] = 5;
		m_tetroCubeGridPositions[3][0] = 0; m_tetroCubeGridPositions[3][1] = 6;
		break;
	}

	//Set cubes at active piece to hold new tetromino information
	for (int i = 0; i < 4; i++)
	{
		m_cubeGrid.at(m_tetroCubeGridPositions[i][0]).at(m_tetroCubeGridPositions[i][1])->SetActive(true);
		m_cubeGrid.at(m_tetroCubeGridPositions[i][0]).at(m_tetroCubeGridPositions[i][1])->SetColor(m_tetroColourR, m_tetroColourG, m_tetroColourB);
	}

}

void
Tetrion::MovePiece(MoveDirection moveDirection)
{
	//Called from InputHandler to shift the piece left or right on user command
	bool canMove = true;

	switch (moveDirection)
	{
	case MoveDirection::LEFT:
		//Need to check if the piece can move any more left
		for (int cube = 0; cube < 4; cube++)
		{
			//If cube row position minus 1 is below 0 then cant move any further left
			if ((m_tetroCubeGridPositions[cube][1] - 1) < 0) //Checking column index
			{
				canMove = false;
			}
		}
		//TODO need to make so that the piece cant move left or right and combine into any existing

		if (canMove)
		{
			ShiftPiece(MoveDirection::LEFT);
		}
		break;
	case MoveDirection::RIGHT:
		//Need to check if the piece can move any more right
		for (int cube = 0; cube < 4; cube++)
		{
			//If cube row position plus 1 is over 9 then cant move any further right
			if ((m_tetroCubeGridPositions[cube][1] + 1) > 9) //Checking column index
			{
				canMove = false;
			}
		}
		
		if (canMove)
		{
			ShiftPiece(MoveDirection::RIGHT);
		}
		break;
	}
}

void 
Tetrion::ShiftPiece(MoveDirection moveDirection)
{
	//Remove existing cubes 
	for (int cube = 0; cube < 4; cube++)
	{
		//Make the previous cubes redundant
		m_cubeGrid.at(m_tetroCubeGridPositions[cube][0]).at(m_tetroCubeGridPositions[cube][1])->SetActive(false);
	}
	//Set new cubes to active
	for (int cube = 0; cube < 4; cube++)
	{

		//Move the piece in the desired direction
		switch (moveDirection)
		{
		case MoveDirection::DOWN:
			m_tetroCubeGridPositions[cube][0]++;
			break;
		case MoveDirection::LEFT:
			m_tetroCubeGridPositions[cube][1]--;
			break;
		case MoveDirection::RIGHT:
			m_tetroCubeGridPositions[cube][1]++;
			break;
		}

		//Employ the new cubes and give them the existing color
		m_cubeGrid.at(m_tetroCubeGridPositions[cube][0]).at(m_tetroCubeGridPositions[cube][1])->SetActive(true);
		m_cubeGrid.at(m_tetroCubeGridPositions[cube][0]).at(m_tetroCubeGridPositions[cube][1])->SetColor(m_tetroColourR, m_tetroColourG, m_tetroColourB);
	}
}

bool
Tetrion::CheckIfPieceCanFall()
{
	for (int cubeIndex = 0; cubeIndex < 4; cubeIndex++)
	{
		//First check if cube is at bottom of the grid
		if (m_tetroCubeGridPositions[cubeIndex][0] >= 21)
		{
			return false;
		}
		else if (m_tetroCubeGridPositions[cubeIndex][0] <= 20)
		{
			//Now IF the cube isnt on the lowest row,
			//Check if there are any cubes below any of the current cube
			Cube* cubeBelow = m_cubeGrid.at(m_tetroCubeGridPositions[cubeIndex][0] + 1).at(m_tetroCubeGridPositions[cubeIndex][1]);
			if (cubeBelow->GetActive())
			{
				//If there is a cube below, must be a part of the shape or return false
				int cubeOccurenceCount = 0;

				for (int tetroCube = 0; tetroCube < 4; tetroCube++)
				{
					//Check if the tetromino cubes position match the one in question
					Cube* tetroSection = m_cubeGrid.at(m_tetroCubeGridPositions[tetroCube][0]).at(m_tetroCubeGridPositions[tetroCube][1]);
					if ((tetroSection->GetPositionX() == cubeBelow->GetPositionX())
						&& (tetroSection->GetPositionY() == cubeBelow->GetPositionY()))
					{
						cubeOccurenceCount++;
					}
				}

				//Now if cube occurence count is above 1, then the cube in question belongs to tetromino
				if (cubeOccurenceCount < 1)
				{
					return false;
				}
			}
		}
	}
	//Return true if nothing blocking shape
	return true;
}

void
Tetrion::FallPiece()
{
	ShiftPiece(MoveDirection::DOWN);
}

//TODO rotation algorithms
void
Tetrion::RotatePiece(MoveDirection movementDirection)
{
	//Rotate piece on pivot point
	//All pieces pivot points are at index 3, except I piece index is 2
	int pivotCubeIndex = 0;

	//Update rotation state number (1-4) based on desired movement direction parameter
	if (movementDirection == MoveDirection::LEFT)
	{
		m_rotationState--;
		if (m_rotationState < 1) { m_rotationState = 4; }
	}
	else if (movementDirection == MoveDirection::RIGHT)
	{
		m_rotationState++;
		if (m_rotationState > 4) { m_rotationState = 1; }
	}

	//Remove existing cubes 
	if (m_currentTetromino != Tetrominos::O)
	{
		for (int cube = 0; cube < 4; cube++)
		{
			//Make the previous cubes redundant
			m_cubeGrid.at(m_tetroCubeGridPositions[cube][0]).at(m_tetroCubeGridPositions[cube][1])->SetActive(false);
		}
	}
	
	//Temp 4x2 array for position keeping
	int tempCubePositions[4][2];

	//Get row and col of the pivot cube
	if (m_currentTetromino == Tetrominos::I) { pivotCubeIndex = 2; }
	else { pivotCubeIndex = 3; }

	int pivotRow = m_tetroCubeGridPositions[pivotCubeIndex - 1][0];
	int pivotCol = m_tetroCubeGridPositions[pivotCubeIndex - 1][1];

	//Set pivot position values
	tempCubePositions[pivotCubeIndex - 1][0] = pivotRow;
	tempCubePositions[pivotCubeIndex - 1][1] = pivotCol;

	
	
	//Update new cube positions based on rotation
	switch (m_currentTetromino)
	{
	case Tetrominos::O:
		
		//Shouldnt be able to rotate square
		break;
	case Tetrominos::S:
		switch (m_rotationState)
		{
		case 1:
			//Cube 1 - 1 left and 1 down from pivot
			tempCubePositions[0][0] = pivotRow + 1;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - below pivot
			tempCubePositions[1][0] = pivotRow + 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - right of pivot
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol + 1;
			break;
		case 2:
			//Cube 1 - 1 left and 1 above pivot
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - left of pivot
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol - 1;
			//Cube 4 - below pivot
			tempCubePositions[3][0] = pivotRow + 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		case 3:
			//Cube 1 - 1 right and 1 above pivot
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol + 1;
			//Cube 2 - above pivot
			tempCubePositions[1][0] = pivotRow - 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - left of pivot
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol - 1;
			break;
		case 4:
			//Cube 1 - 1 right and 1 below pivot
			tempCubePositions[0][0] = pivotRow + 1;
			tempCubePositions[0][1] = pivotCol + 1;
			//Cube 2 - right to pivot	
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol + 1;
			//Cube 4 - above pivot
			tempCubePositions[3][0] = pivotRow - 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		}
		break;
	case Tetrominos::Z:
		switch (m_rotationState)
		{
		case 1:
			//Cube 1 - 1 right and 1 down from pivot
			tempCubePositions[0][0] = pivotRow + 1;
			tempCubePositions[0][1] = pivotCol + 1;
			//Cube 2 - below pivot
			tempCubePositions[1][0] = pivotRow + 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - left of pivot
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol - 1;
			break;
		case 2:
			//Cube 1 - left and below pivot
			tempCubePositions[0][0] = pivotRow + 1;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - left of pivot
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol - 1;
			//Cube 4 - above pivot
			tempCubePositions[3][0] = pivotRow - 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		case 3:
			//Cube 1 - left and above pivot
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - above pivot
			tempCubePositions[1][0] = pivotRow - 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - right of pivot
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol + 1;
			break;
		case 4:
			//Cube 1 - right, up
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol + 1;
			//Cube 2 - right
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol + 1;
			//Cube 4 - below
			tempCubePositions[3][0] = pivotRow + 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		}
		break;
	case Tetrominos::L:
		switch (m_rotationState)
		{
		case 1:
			//Cube 1 - right, down
			tempCubePositions[0][0] = pivotRow + 1;
			tempCubePositions[0][1] = pivotCol + 1;
			//Cube 2 - below
			tempCubePositions[1][0] = pivotRow + 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - above
			tempCubePositions[3][0] = pivotRow - 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		case 2:
			//Cube 1 - left, down
			tempCubePositions[0][0] = pivotRow + 1;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - left
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol - 1;
			//Cube 4 - right
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol + 1;
			break;
		case 3:
			//Cube 1 - left, up
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - above
			tempCubePositions[1][0] = pivotRow - 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - below
			tempCubePositions[3][0] = pivotRow + 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		case 4:
			//Cube 1 - right, above
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol + 1;
			//Cube 2 - right
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol + 1;
			//Cube 4 - left
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol - 1;
			break;
		}
		break;
	case Tetrominos::J:
		switch (m_rotationState)
		{
		case 1:
			//Cube 1 - left, down
			tempCubePositions[0][0] = pivotRow + 1;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - below
			tempCubePositions[1][0] = pivotRow + 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - above
			tempCubePositions[3][0] = pivotRow - 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		case 2:
			//Cube 1 - left, up
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - left
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol - 1;
			//Cube 4 - right
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol + 1;
			break;
		case 3:
			//Cube 1 - right, up
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol + 1;
			//Cube 2 - above
			tempCubePositions[1][0] = pivotRow - 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - below
			tempCubePositions[3][0] = pivotRow + 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		case 4:
			//Cube 1 - left, up
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - left
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol - 1;
			//Cube 4 - right
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol + 1;
			break;
		}
		break;
	case Tetrominos::T:
		switch (m_rotationState)
		{
		case 1:
			//Cube 1 - below
			tempCubePositions[0][0] = pivotRow + 1;
			tempCubePositions[0][1] = pivotCol;
			//Cube 2 - left
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol - 1;
			//Cube 4 - right
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol + 1;
			break;
		case 2:
			//Cube 1 - left
			tempCubePositions[0][0] = pivotRow;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 2 - above
			tempCubePositions[1][0] = pivotRow - 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - below
			tempCubePositions[3][0] = pivotRow + 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		case 3:
			//Cube 1 - above
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol;
			//Cube 2 - right
			tempCubePositions[1][0] = pivotRow;
			tempCubePositions[1][1] = pivotCol - 1;
			//Cube 4 - left
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol + 1;
			break;
		case 4:
			//Cube 1 - right
			tempCubePositions[0][0] = pivotRow;
			tempCubePositions[0][1] = pivotCol + 1;
			//Cube 2 - below
			tempCubePositions[1][0] = pivotRow + 1;
			tempCubePositions[1][1] = pivotCol;
			//Cube 4 - above
			tempCubePositions[3][0] = pivotRow - 1;
			tempCubePositions[3][1] = pivotCol;
			break;
		}
		break;
	case Tetrominos::I:
		switch (m_rotationState)
		{
		case 1:
			//Cube 1 - below
			tempCubePositions[0][0] = pivotRow + 1;
			tempCubePositions[0][1] = pivotCol;
			//Cube 3 - above
			tempCubePositions[2][0] = pivotRow - 1;
			tempCubePositions[2][1] = pivotCol;
			//Cube 4 - 2 above
			tempCubePositions[3][0] = pivotRow - 2;
			tempCubePositions[3][1] = pivotCol;
			break;
		case 2:
			//Cube 1 - left
			tempCubePositions[0][0] = pivotRow;
			tempCubePositions[0][1] = pivotCol - 1;
			//Cube 3 - right
			tempCubePositions[2][0] = pivotRow;
			tempCubePositions[2][1] = pivotCol + 1;
			//Cube 4 - 2 right
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol + 2;
			break;
		case 3:
			//Cube 1 - above
			tempCubePositions[0][0] = pivotRow - 1;
			tempCubePositions[0][1] = pivotCol;
			//Cube 3 - below
			tempCubePositions[2][0] = pivotRow + 1;
			tempCubePositions[2][1] = pivotCol;
			//Cube 4 - 2 below
			tempCubePositions[3][0] = pivotRow + 2;
			tempCubePositions[3][1] = pivotCol;
			break;
		case 4:
			//Cube 1 - right
			tempCubePositions[0][0] = pivotRow;
			tempCubePositions[0][1] = pivotCol + 1;
			//Cube 3 - left
			tempCubePositions[2][0] = pivotRow;
			tempCubePositions[2][1] = pivotCol - 1;
			//Cube 4 - 2 left
			tempCubePositions[3][0] = pivotRow;
			tempCubePositions[3][1] = pivotCol - 2;
			break;
		}
		break;
	}
	if (m_currentTetromino != Tetrominos::O)
	{
		//Check if tetromino has rotated out of bounds and need to be shifted
		for (int cube = 0; cube < 4; cube++)
		{
			if (tempCubePositions[cube][0] < 0)
			{
				//Beyond bottom of grid - shift up
				while (tempCubePositions[cube][0] > 21)
				{
					for (int tetroCube = 0; tetroCube < 4; tetroCube++)
					{
						tempCubePositions[tetroCube][0] -= 1;
					}
				} 
				break;
			}
			else if (tempCubePositions[cube][0] > 19)
			{
				//Beyond top of grid - shift down
				while (tempCubePositions[cube][0] < 2)
				{
					for (int tetroCube = 0; tetroCube < 4; tetroCube++)
					{
						tempCubePositions[tetroCube][0] += 1;
					}
				} 
				break;
			}
			else if (tempCubePositions[cube][1] < 0)
			{
				//Beyond left of grid - shift right
				while (tempCubePositions[cube][1] < 0)
				{
					for (int tetroCube = 0; tetroCube < 4; tetroCube++)
					{
						tempCubePositions[tetroCube][1] += 1;
					}
				} 
				break;
			}
			else if (tempCubePositions[cube][1] > 9)
			{
				//Beyond right of grid - shift left
				while (tempCubePositions[cube][1] >= 9)
				{
					for (int tetroCube = 0; tetroCube < 4; tetroCube++)
					{
						tempCubePositions[tetroCube][1] -= 1;
					}
				} 
				break;
			}
		}

		//Check if any of these new temp cubes have already active pieces that are not part of tetromino
		bool existingCubeInTheWay = false;
		for (int cube = 0; cube < 4; cube++)
		{
			if (m_cubeGrid.at(tempCubePositions[cube][0]).at(tempCubePositions[cube][1])->GetActive())
			{
				int notPartOfTetrominoCubes = 4;
				//Active cube, check if its part of the tetromino
				for (int tetroCube = 0; tetroCube < 4; tetroCube++)
				{
					if (m_tetroCubeGridPositions[tetroCube][0] == tempCubePositions[tetroCube][0])
					{
						if (m_tetroCubeGridPositions[tetroCube][1] == tempCubePositions[tetroCube][1])
						{
							notPartOfTetrominoCubes--;
						}
					}
				}
				if (notPartOfTetrominoCubes > 0)
				{
					existingCubeInTheWay = true;
				}
			}
		}

		//Set cubes at active piece to hold new tetromino information if no existing cubes in the way
		if (!existingCubeInTheWay)
		{
			for (int i = 0; i < 4; i++)
			{
				m_tetroCubeGridPositions[i][0] = tempCubePositions[i][0];
				m_tetroCubeGridPositions[i][1] = tempCubePositions[i][1];

				m_cubeGrid.at(m_tetroCubeGridPositions[i][0]).at(m_tetroCubeGridPositions[i][1])->SetActive(true);
				m_cubeGrid.at(m_tetroCubeGridPositions[i][0]).at(m_tetroCubeGridPositions[i][1])->SetColor(m_tetroColourR, m_tetroColourG, m_tetroColourB);
			}
		}
	}

	
}

bool 
Tetrion::CheckIfPieceSpawnPossible()
{
	int activeCubeCount = 0;

	//Need to count how many cubes are set active in rows 20 and 21
	for (int row = 0; row < 2; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			bool cubeActive = m_cubeGrid.at(row).at(col)->GetActive();
			if (cubeActive)
			{
				activeCubeCount++;
			}
		}
	}

	//If there are any cubes in the buffer, game over - no pieces can spawn
	if (activeCubeCount > 1)
	{
		return false;
	}
	return true;
}

void
Tetrion::LockPiece()
{
	//Save color into the cubes being offloaded
	for (int cube = 0; cube < 4; cube++)
	{
		m_cubeGrid.at(m_tetroCubeGridPositions[cube][0]).at(m_tetroCubeGridPositions[cube][1])->SaveColor(m_tetroColourR, m_tetroColourG, m_tetroColourB);

	}
}

int 
Tetrion::CheckForFullRows()
{
	int rowsRemoved = 0;

	//Loop over all rows
	for (int row = 0; row < 22; row++)
	{
		bool fullRow = true;
		//Loop over all cubes in the row
		for (int cubeInRow = 0; cubeInRow < 10; cubeInRow++)
		{
			if (m_cubeGrid.at(row).at(cubeInRow)->GetActive() == false)
			{
				fullRow = false;
			}
		}

		//If fullRow has not been set to false, then it is full and needs to be removed
		if (fullRow)
		{
			RemoveFullRow(row);
			rowsRemoved++;
		}
	}

	return rowsRemoved;
}

void 
Tetrion::RemoveFullRow(int rowIndex)
{
	//Remove this row
	for (int col = 0; col < 10; col++)
	{
		m_cubeGrid.at(rowIndex).at(col)->SetActive(false);
	}
	
	//Shift any rows above down by 1
	for (int row = rowIndex - 1; row > 2; row--)
	{
		for (int col = 0; col < 10; col++)
		{
			if (m_cubeGrid.at(row).at(col)->GetActive())
			{
				//Switch activeness
				m_cubeGrid.at(row).at(col)->SetActive(false);
				m_cubeGrid.at(row + 1).at(col)->SetActive(true);
				//Change color
				int red = m_cubeGrid.at(row).at(col)->GetColorR();
				int green = m_cubeGrid.at(row).at(col)->GetColorG();
				int blue = m_cubeGrid.at(row).at(col)->GetColorB();
				m_cubeGrid.at(row + 1).at(col)->SetColor(red, green, blue);
			}
		}
	}
}

bool 
Tetrion::CheckForPerfectClear()
{
	int cubesOnGridCount = 0;
	for (int row = 2; row <= 21; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			if (m_cubeGrid.at(row).at(col)->GetActive())
			{
				cubesOnGridCount++;
			}
		}
	}
	return (cubesOnGridCount <= 0); //Returns TRUE if no cubes on Tetrion (perfect clear)
}