#include "Piece.h"

using namespace std;

Piece::Piece(std::default_random_engine& generator, std::discrete_distribution<int> distribution) : orientation{}, positions{}
{
	// Assign characters
	for (int i = 0; i < 3; i++)
	{
		int randomPos = distribution(generator);
		characters[i] = ALPHABET[randomPos];
	}

	// Assign orientation
	int randomOrientation = rand() % 4;
	switch (randomOrientation)
	{
	case 0:
		orientation = Up;
		break;
	case 1:
		orientation = Right;
		break;
	case 2:
		orientation = Down;
		break;
	case 3:
		orientation = Left;
		break;
	}

	// Assign shape
	int randomShape = rand() % 2;
	shape = (randomShape == 0) ? L : Line;

	// Assign positions based on orientation and shape
	// Preferably this would be dynamic, based on GRID_WIDTH, but just using magic numbers for now
	switch (orientation)
	{
	case Up:
		positions[0][0] = (shape == L) ? 4 : 4;
		positions[0][1] = (shape == L) ? 0 : 0;
		positions[1][0] = (shape == L) ? 4 : 4;
		positions[1][1] = (shape == L) ? 1 : 1;
		positions[2][0] = (shape == L) ? 5 : 4;
		positions[2][1] = (shape == L) ? 1 : 2;
		break;
	case Right:
		positions[0][0] = (shape == L) ? 5 : 5;
		positions[0][1] = (shape == L) ? 0 : 0;
		positions[1][0] = (shape == L) ? 4 : 4;
		positions[1][1] = (shape == L) ? 0 : 0;
		positions[2][0] = (shape == L) ? 4 : 3;
		positions[2][1] = (shape == L) ? 1 : 0;
		break;
	case Down:
		positions[0][0] = (shape == L) ? 5 : 5;
		positions[0][1] = (shape == L) ? 1 : 2;
		positions[1][0] = (shape == L) ? 5 : 5;
		positions[1][1] = (shape == L) ? 0 : 1;
		positions[2][0] = (shape == L) ? 4 : 5;
		positions[2][1] = (shape == L) ? 0 : 0;
		break;
	case Left:
		positions[0][0] = (shape == L) ? 4 : 4; // Might change positions for line shape
		positions[0][1] = (shape == L) ? 1 : 0;
		positions[1][0] = (shape == L) ? 5 : 5;
		positions[1][1] = (shape == L) ? 1 : 0;
		positions[2][0] = (shape == L) ? 5 : 6;
		positions[2][1] = (shape == L) ? 0 : 0;
		break;
	}
}

bool Piece::Drop(const char grid[GRID_HEIGHT][GRID_WIDTH])
{
	switch (orientation)
	{
	case Up:
		if (shape == L)
		{
			// Drop L one place
			if (grid[positions[1][1] + 1][positions[1][0]] == ' ' && grid[positions[2][1] + 1][positions[2][0]] == ' ')
			{
				for (int i = 0; i < 3; i++)
				{
					positions[i][1]++;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			// Drop Line one place
			if (grid[positions[2][1] + 1][positions[2][0]] == ' ')
			{
				for (int i = 0; i < 3; i++)
				{
					positions[i][1]++;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
	case Right:
		if (shape == L)
		{
			// Drop L one place
			if (grid[positions[2][1] + 1][positions[2][0]] == ' ' && grid[positions[0][1] + 1][positions[0][0]] == ' ')
			{
				for (int i = 0; i < 3; i++)
				{
					positions[i][1]++;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			// Drop Line one place
			if (grid[positions[2][1] + 1][positions[2][0]] == ' ' && grid[positions[1][1] + 1][positions[1][0]] == ' ' && grid[positions[0][1] + 1][positions[0][0]] == ' ')
			{
				for (int i = 0; i < 3; i++)
				{
					positions[i][1]++;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
	case Down:
		if (shape == L)
		{
			// Drop L one place
			if (grid[positions[2][1] + 1][positions[2][0]] == ' ' && grid[positions[0][1] + 1][positions[0][0]] == ' ')
			{
				for (int i = 0; i < 3; i++)
				{
					positions[i][1]++;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			// Drop Line one place
			if (grid[positions[0][1] + 1][positions[0][0]] == ' ')
			{
				for (int i = 0; i < 3; i++)
				{
					positions[i][1]++;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
	case Left:
		if (shape == L)
		{
			// Drop L one place
			if (grid[positions[0][1] + 1][positions[0][0]] == ' ' && grid[positions[1][1] + 1][positions[1][0]] == ' ')
			{
				for (int i = 0; i < 3; i++)
				{
					positions[i][1]++;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			// Drop Line one place
			if (grid[positions[2][1] + 1][positions[2][0]] == ' ' && grid[positions[1][1] + 1][positions[1][0]] == ' ' && grid[positions[0][1] + 1][positions[0][0]] == ' ')
			{
				for (int i = 0; i < 3; i++)
				{
					positions[i][1]++;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
	}
}

bool Piece::MoveLeft(const char grid[GRID_HEIGHT][GRID_WIDTH])
{
	switch (orientation)
	{
	case Up:
		if ((shape == L && positions[0][0] > 0 && grid[positions[0][1]][positions[0][0] - 1] == ' ' && grid[positions[1][1]][positions[1][0] - 1] == ' ')
			|| (shape == Line && positions[0][0] > 0 && grid[positions[0][1]][positions[0][0] - 1] == ' ' && grid[positions[1][1]][positions[1][0] - 1] == ' ' && grid[positions[2][1]][positions[2][0] - 1] == ' '))
		{
			for (int i = 0; i < 3; i++)
			{
				positions[i][0]--;
			}
			return true;
		}
		break;
	case Right:
		if ((shape == L && positions[1][0] > 0 && grid[positions[1][1]][positions[1][0] - 1] == ' ' && grid[positions[2][1]][positions[2][0] - 1] == ' ')
			|| (shape == Line && positions[2][0] > 0 && grid[positions[2][1]][positions[2][0] - 1] == ' '))
		{
			for (int i = 0; i < 3; i++)
			{
				positions[i][0]--;
			}
			return true;
		}
		break;
	case Down:
		if ((shape == L && positions[2][0] > 0 && grid[positions[2][1]][positions[2][0] - 1] == ' ' && grid[positions[2][1] + 1][positions[2][0]] == ' ')
			|| (shape == Line && positions[0][0] > 0 && grid[positions[0][1]][positions[0][0] - 1] == ' ' && grid[positions[1][1]][positions[1][0] - 1] == ' ' && grid[positions[2][1]][positions[2][0] - 1] == ' '))
		{
			for (int i = 0; i < 3; i++)
			{
				positions[i][0]--;
			}
			return true;
		}
		break;
	case Left:
		if ((shape == L && positions[0][0] > 0 && grid[positions[0][1]][positions[0][0] - 1] == ' ' && grid[positions[0][1] - 1][positions[0][0]] == ' ')
			|| (shape == Line && positions[0][0] > 0 && grid[positions[0][1]][positions[0][0] - 1] == ' '))
		{
			for (int i = 0; i < 3; i++)
			{
				positions[i][0]--;
			}
			return true;
		}
		break;
	}
	return false;
}

bool Piece::MoveRight(const char grid[GRID_HEIGHT][GRID_WIDTH])
{
	switch (orientation)
	{
	case Up:
		if ((shape == L && positions[2][0] < GRID_WIDTH - 1 && grid[positions[2][1]][positions[2][0] + 1] == ' ' && grid[positions[2][1] - 1][positions[2][0]] == ' ')
			|| (shape == Line && positions[0][0] < GRID_WIDTH - 1 && grid[positions[0][1]][positions[0][0] + 1] == ' ' && grid[positions[1][1]][positions[1][0] + 1] == ' ' && grid[positions[2][1]][positions[2][0] + 1] == ' '))
		{
			for (int i = 0; i < 3; i++)
			{
				positions[i][0]++;
			}
			return true;
		}
		break;
	case Right:
		if ((shape == L && positions[0][0] < GRID_WIDTH - 1 && grid[positions[0][1]][positions[0][0] + 1] == ' ' && grid[positions[0][1] + 1][positions[0][0]] == ' ')
			|| (shape == Line && positions[0][0] < GRID_WIDTH - 1 && grid[positions[0][1]][positions[0][0] + 1] == ' '))
		{
			for (int i = 0; i < 3; i++)
			{
				positions[i][0]++;
			}
			return true;
		}
		break;
	case Down:
		if ((shape == L && positions[0][0] < GRID_WIDTH - 1 && grid[positions[0][1]][positions[0][0] + 1] == ' ' && grid[positions[1][1]][positions[1][0] + 1] == ' ')
			|| (shape == Line && positions[0][0] < GRID_WIDTH - 1 && grid[positions[0][1]][positions[0][0] + 1] == ' ' && grid[positions[1][1]][positions[1][0] + 1] == ' ' && grid[positions[2][1]][positions[2][0] + 1] == ' '))
		{
			for (int i = 0; i < 3; i++)
			{
				positions[i][0]++;
			}
			return true;
		}
		break;
	case Left:
		if ((shape == L && positions[1][0] < GRID_WIDTH - 1 && grid[positions[1][1]][positions[1][0] + 1] == ' ' && grid[positions[2][1]][positions[2][0] + 1] == ' ')
			|| (shape == Line && positions[2][0] < GRID_WIDTH - 1 && grid[positions[2][1]][positions[2][0] + 1] == ' '))
		{
			for (int i = 0; i < 3; i++)
			{
				positions[i][0]++;
			}
			return true;
		}
		break;
	}
	return false;
}

void Piece::DropInstantly(char grid[GRID_HEIGHT][GRID_WIDTH])
{
	switch (orientation)
	{
	case Up:
		if (shape == L)
		{
			// Drop the shortest distance.
			int distance1 = 0;
			while (positions[1][1] + 1 + distance1 < GRID_HEIGHT && grid[positions[1][1] + 1 + distance1][positions[1][0]] == ' ')
			{
				distance1++;
			}

			int distance2 = 0;
			while (positions[2][1] + 1 + distance2 < GRID_HEIGHT && grid[positions[2][1] + 1 + distance2][positions[2][0]] == ' ')
			{
				distance2++;
			}

			int minDistance = (distance1 < distance2) ? distance1 : distance2;
			for (int i = 0; i < 3; i++)
			{
				positions[i][1] += minDistance;
			}
		}
		else
		{
			// Line shape
			int distance = 0;
			while (positions[2][1] + 1 + distance < GRID_HEIGHT && grid[positions[2][1] + 1 + distance][positions[2][0]] == ' ')
			{
				distance++;
			}
			for (int i = 0; i < 3; i++)
			{
				positions[i][1] += distance;
			}
		}
		break;
	case Right:
		if (shape == L)
		{
			// Drop the shortest distance
			int distance1 = 0;
			while (positions[2][1] + 1 + distance1 < GRID_HEIGHT && grid[positions[2][1] + 1 + distance1][positions[2][0]] == ' ')
			{
				distance1++;
			}

			int distance2 = 0;
			while (positions[0][1] + 1 + distance2 < GRID_HEIGHT && grid[positions[0][1] + 1 + distance2][positions[0][0]] == ' ')
			{
				distance2++;
			}

			int minDistance = (distance1 < distance2) ? distance1 : distance2;
			for (int i = 0; i < 3; i++)
			{
				positions[i][1] += minDistance;
			}
		}
		else
		{
			// Line shape
			int distances[3] = {0, 0, 0};
			for (int i = 0; i < 3; i++)
			{
				while (positions[i][1] + 1 + distances[i] < GRID_HEIGHT && grid[positions[i][1] + 1 + distances[i]][positions[i][0]] == ' ')
				{
					distances[i]++;
				}
			}
			
			int minDistance = 999;
			for (int i = 0; i < 3; i++)
				minDistance = (distances[i] < minDistance) ? distances[i] : minDistance;

			for (int i = 0; i < 3; i++)
			{
				positions[i][1] += minDistance;
			}
		}
		break;
	case Down:
		if (shape == L)
		{
			// Drop the shortest distance
			int distance1 = 0;
			while (positions[2][1] + 1 + distance1 < GRID_HEIGHT && grid[positions[2][1] + 1 + distance1][positions[2][0]] == ' ')
			{
				distance1++;
			}

			int distance2 = 0;
			while (positions[0][1] + 1 + distance2 < GRID_HEIGHT && grid[positions[0][1] + 1 + distance2][positions[0][0]] == ' ')
			{
				distance2++;
			}

			int minDistance = (distance1 < distance2) ? distance1 : distance2;
			for (int i = 0; i < 3; i++)
			{
				positions[i][1] += minDistance;
			}
		}
		else
		{
			// Line shape
			int distance = 0;
			while (positions[0][1] + 1 + distance < GRID_HEIGHT && grid[positions[0][1] + 1 + distance][positions[0][0]] == ' ')
			{
				distance++;
			}
			for (int i = 0; i < 3; i++)
			{
				positions[i][1] += distance;
			}
		}
		break;
	case Left:
		if (shape == L)
		{
			// Drop the shortest distance
			int distance1 = 0;
			while (positions[0][1] + 1 + distance1 < GRID_HEIGHT && grid[positions[0][1] + 1 + distance1][positions[0][0]] == ' ')
			{
				distance1++;
			}

			int distance2 = 0;
			while (positions[1][1] + 1 + distance2 < GRID_HEIGHT && grid[positions[1][1] + 1 + distance2][positions[1][0]] == ' ')
			{
				distance2++;
			}

			int minDistance = (distance1 < distance2) ? distance1 : distance2;
			for (int i = 0; i < 3; i++)
			{
				positions[i][1] += minDistance;
			}
		}
		else
		{
			// Line shape
			int distances[3] = { 0, 0, 0 };
			for (int i = 0; i < 3; i++)
			{
				while (positions[i][1] + 1 + distances[i] < GRID_HEIGHT && grid[positions[i][1] + 1 + distances[i]][positions[i][0]] == ' ')
				{
					distances[i]++;
				}
			}

			int minDistance = 999;
			for (int i = 0; i < 3; i++)
				minDistance = (distances[i] < minDistance) ? distances[i] : minDistance;

			for (int i = 0; i < 3; i++)
			{
				positions[i][1] += minDistance;
			}
		}
		break;
	}
}

bool Piece::RotateClockwise(const char grid[GRID_HEIGHT][GRID_WIDTH])
{
	// Assume rotation around character at index 1 (characters[1])
	Orientation desiredOrientation;
	switch (orientation)
	{
	case Up:
		desiredOrientation = Right;
		if (shape == L)
		{
			// Attempt to rotate the L
			if (positions[1][1] + 1 < GRID_HEIGHT && grid[positions[1][1] + 1][positions[1][0]] == ' ')
			{
				positions[2][0]--;
				positions[2][1]++;
				positions[0][0]++;
				positions[0][1]++;
				orientation = desiredOrientation;
				return true;
			}
		}
		else
		{
			// Attempt to rotate the Line
			if (positions[1][0] - 1 >= 0 && positions[1][0] + 1 < GRID_WIDTH
				&& grid[positions[1][1]][positions[1][0] - 1] == ' ' && grid[positions[1][1]][positions[1][0] + 1] == ' ')
			{
				positions[2][0]--;
				positions[2][1]--;
				positions[0][0]++;
				positions[0][1]++;
				orientation = desiredOrientation;
				return true;
			}
		}
		break;
	case Right:
		desiredOrientation = Down;
		if (shape == L)
		{
			// Attempt to rotate the L
			if (positions[1][0] - 1 >= 0 && grid[positions[1][1]][positions[1][0] - 1] == ' ')
			{
				positions[2][0]--;
				positions[2][1]--;
				positions[0][0]--;
				positions[0][1]++;
				orientation = desiredOrientation;
				return true;
			}
		}
		else
		{
			// Attempt to rotate the Line
			if (positions[1][1] + 1 < GRID_HEIGHT && positions[1][1] - 1 >= 0
				&& grid[positions[1][1] + 1][positions[1][0]] == ' ' && grid[positions[1][1] - 1][positions[1][0]] == ' ')
			{
				positions[2][0]++;
				positions[2][1]--;
				positions[0][0]--;
				positions[0][1]++;
				orientation = desiredOrientation;
				return true;
			}
		}
		break;
	case Down:
		desiredOrientation = Left;
		if (shape == L)
		{
			// Attempt to rotate the L
			if (positions[1][1] - 1 >= 0 && grid[positions[1][1] - 1][positions[1][0]] == ' ')
			{
				positions[2][0]++;
				positions[2][1]--;
				positions[0][0]--;
				positions[0][1]--;
				orientation = desiredOrientation;
				return true;
			}
		}
		else
		{
			// Attempt to rotate the Line
			if (positions[1][0] + 1 < GRID_WIDTH && positions[1][0] - 1 >= 0
				&& grid[positions[1][1]][positions[1][0] + 1] == ' ' && grid[positions[1][1]][positions[1][0] - 1] == ' ')
			{
				positions[2][0]++;
				positions[2][1]++;
				positions[0][0]--;
				positions[0][1]--;
				orientation = desiredOrientation;
				return true;
			}
		}
		break;
	case Left:
		desiredOrientation = Up;
		if (shape == L)
		{
			// Attempt to rotate the L
			if (positions[1][0] + 1 < GRID_WIDTH && grid[positions[1][1]][positions[1][0] + 1] == ' ')
			{
				positions[2][0]++;
				positions[2][1]++;
				positions[0][0]++;
				positions[0][1]--;
				orientation = desiredOrientation;
				return true;
			}
		}
		else
		{
			// Attempt to rotate the Line
			if (positions[1][1] + 1 < GRID_HEIGHT && positions[1][1] - 1 >= 0
				&& grid[positions[1][1] + 1][positions[1][0]] == ' ' && grid[positions[1][1] - 1][positions[1][0]] == ' ')
			{
				positions[2][0]--;
				positions[2][1]++;
				positions[0][0]++;
				positions[0][1]--;
				orientation = desiredOrientation;
				return true;
			}
		}
		break;

	}
	return false;
}

bool Piece::RotateAnticlockwise(const char grid[GRID_HEIGHT][GRID_WIDTH])
{
	// Assume rotation around character at index 1 (characters[1])
	Orientation desiredOrientation;
	switch (orientation)
	{
	case Up:
		desiredOrientation = Left;
		if (shape == L)
		{
			// Attempt to rotate the L
			if (positions[1][0] - 1 >= 0 && grid[positions[1][1]][positions[1][0] - 1] == ' ')
			{
				positions[0][0]--;
				positions[0][1]++;
				positions[2][0]--;
				positions[2][1]--;
				orientation = desiredOrientation;
				return true;
			}
		}
		else
		{
			// Attempt to rotate the Line
			if (positions[1][0] - 1 >= 0 && positions[1][0] + 1 < GRID_WIDTH
				&& grid[positions[1][1]][positions[1][0] - 1] == ' ' && grid[positions[1][1]][positions[1][0] + 1] == ' ')
			{
				positions[0][0]--;
				positions[0][1]++;
				positions[2][0]++;
				positions[2][1]--;
				orientation = desiredOrientation;
				return true;
			}
		}
		break;
	case Right:
		desiredOrientation = Up;
		if (shape == L)
		{
			// Attempt to rotate the L
			if (positions[1][1] - 1 >= 0 && grid[positions[1][1] - 1][positions[1][0]] == ' ')
			{
				positions[0][0]--;
				positions[0][1]--;
				positions[2][0]++;
				positions[2][1]--;
				orientation = desiredOrientation;
				return true;
			}
		}
		else
		{
			// Attempt to rotate the Line
			if (positions[1][1] + 1 < GRID_HEIGHT && positions[1][1] - 1 >= 0
				&& grid[positions[1][1] + 1][positions[1][0]] == ' ' && grid[positions[1][1] - 1][positions[1][0]] == ' ')
			{
				positions[0][0]--;
				positions[0][1]--;
				positions[2][0]++;
				positions[2][1]++;
				orientation = desiredOrientation;
				return true;
			}
		}
		break;
	case Down:
		desiredOrientation = Right;
		if (shape == L)
		{
			// Attempt to rotate the L
			if (positions[1][0] + 1 < GRID_WIDTH && grid[positions[1][1]][positions[1][0] + 1] == ' ')
			{
				positions[0][0]++;
				positions[0][1]--;
				positions[2][0]++;
				positions[2][1]++;
				orientation = desiredOrientation;
				return true;
			}
		}
		else
		{
			// Attempt to rotate the Line
			if (positions[1][0] + 1 < GRID_WIDTH && positions[1][0] - 1 >= 0
				&& grid[positions[1][1]][positions[1][0] + 1] == ' ' && grid[positions[1][1]][positions[1][0] - 1] == ' ')
			{
				positions[0][0]++;
				positions[0][1]--;
				positions[2][0]--;
				positions[2][1]++;
				orientation = desiredOrientation;
				return true;
			}
		}
		break;
	case Left:
		desiredOrientation = Down;
		if (shape == L)
		{
			// Attempt to rotate the L
			if (positions[1][1] + 1 < GRID_HEIGHT && grid[positions[1][1] + 1][positions[1][0]] == ' ')
			{
				positions[0][0]++;
				positions[0][1]++;
				positions[2][0]--;
				positions[2][1]++;
				orientation = desiredOrientation;
				return true;
			}
		}
		else
		{
			// Attempt to rotate the Line
			if (positions[1][1] + 1 < GRID_HEIGHT && positions[1][1] - 1 >= 0
				&& grid[positions[1][1] + 1][positions[1][0]] == ' ' && grid[positions[1][1] - 1][positions[1][0]] == ' ')
			{
				positions[0][0]++;
				positions[0][1]++;
				positions[2][0]--;
				positions[2][1]--;
				orientation = desiredOrientation;
				return true;
			}
		}
		break;

	}
	return false;
}

void Piece::Fix(char grid[GRID_HEIGHT][GRID_WIDTH]) const // Need a function to resolve gravity
{
	for (int i = 0; i < 3; i++)
	{
		grid[positions[i][1]][positions[i][0]] = characters[i];
	}
}

void Piece::ShuffleLetters()
{
	char temp = characters[0];
	characters[0] = characters[1];
	characters[1] = characters[2];
	characters[2] = temp;
}

bool Piece::ChangeShape(const char grid[GRID_HEIGHT][GRID_WIDTH])
{
	switch (orientation)
	{
	case Up:
		if (shape == L)
		{
			// Change L to Line
			if (positions[1][1] + 1 < GRID_HEIGHT && grid[positions[1][1] + 1][positions[1][0]] == ' ')
			{
				positions[2][0]--;
				positions[2][1]++;
				shape = Line;
				return true;
			}
		}
		else
		{
			// Change Line to L
			if (positions[1][0] + 1 < GRID_WIDTH && grid[positions[1][1]][positions[1][0] + 1] == ' ')
			{
				positions[2][0]++;
				positions[2][1]--;
				shape = L;
				return true;
			}
		}
		break;
	case Right:
		if (shape == L)
		{
			// Change L to Line
			if (positions[1][0] - 1 >= 0 && grid[positions[1][1]][positions[1][0] - 1] == ' ')
			{
				positions[2][0]--;
				positions[2][1]--;
				shape = Line;
				return true;
			}
		}
		else
		{
			// Change Line to L
			if (positions[1][1] + 1 < GRID_HEIGHT && grid[positions[1][1] + 1][positions[1][0]] == ' ')
			{
				positions[2][0]++;
				positions[2][1]++;
				shape = L;
				return true;
			}
		}
		break;
	case Down:
		if (shape == L)
		{
			// Change L to Line
			if (positions[1][1] - 1 >= 0 && grid[positions[1][1] - 1][positions[1][0]] == ' ')
			{
				positions[2][0]++;
				positions[2][1]--;
				shape = Line;
				return true;
			}
		}
		else
		{
			// Change Line to L
			if (positions[1][0] - 1 >= 0 && grid[positions[1][1]][positions[1][0] - 1] == ' ')
			{
				positions[2][0]--;
				positions[2][1]++;
				shape = L;
				return true;
			}
		}
		break;
	case Left:
		if (shape == L)
		{
			// Change L to Line
			if (positions[1][0] + 1 < GRID_WIDTH && grid[positions[1][1]][positions[1][0] + 1] == ' ')
			{
				positions[2][0]++;
				positions[2][1]++;
				shape = Line;
				return true;
			}
		}
		else
		{
			// Change Line to L
			if (positions[1][1] - 1 >= 0 && grid[positions[1][1] - 1][positions[1][0]] == ' ')
			{
				positions[2][0]--;
				positions[2][1]--;
				shape = L;
				return true;
			}
		}
		break;
	}
	return false;
}

bool Piece::IsOverlapping(const char grid[GRID_HEIGHT][GRID_WIDTH]) const
{
	for (int i = 0; i < 3; i++)
	{
		if (grid[positions[i][1]][positions[i][0]] != ' ')
		{
			return true;
		}
	}
	return false;
}

char Piece::GetCharacter(const int index) const
{
	try
	{
		return characters[index];
	}
	catch (std::out_of_range& e)
	{
		std::cerr << "Index out of range error: " << e.what() << std::endl;
	}
	return ' ';
}

int Piece::GetCharacterPosition(const int characterIndex, const int positionIndex) const
{
	try
	{
		return positions[characterIndex][positionIndex];
	}
	catch (std::out_of_range& e)
	{
		std::cerr << "Index out of range error: " << e.what() << std::endl;
	}
	return 0;
}