#include "grid.h"

void Grid::load_grid()
{
	srand(time(NULL));

	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			grid[i][j] = rand() % 5;
		}
	}
	vector<int>tmp(10, -1);
	vector<int>tmp_(10, -1);
	while (sequence(tmp))
	{
		update_grid(tmp);
		tmp = tmp_;
	}
}

bool Grid::valid_move(int x0, int y0, int x1, int y1)
{
	if ((x0 + 1 == x1 && y0 == y1) || (x0 - 1 == x1 && y0 == y1) || (x0 == x1 && y0 + 1 == y1) || (x0 == x1 && y0 - 1 == y1))
	{
		swap(grid[x0][y0], grid[x1][y1]);
		return true;
	}
	else
		return false;
}

int Grid::get_shape(int i, int j)
{
	return grid[i][j];
}

bool Grid::sequence(vector<int>& tmp)
{
	int x, y;
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			x = i;
			y = j;
			if (x < GRID_SIZE - 2 && grid[x][y] == grid[x + 1][y] && grid[x][y] == grid[x + 2][y])
			{
				tmp[0] = x; tmp[1] = x + 1; tmp[2] = x + 2;
				tmp[3] = y; tmp[4] = y; tmp[5] = y;
				if (x < GRID_SIZE - 3 && grid[x][y] == grid[x + 3][y])
				{
					tmp[6] = x + 3; tmp[7] = y;

					if (x < GRID_SIZE - 4 && grid[x][y] == grid[x + 4][y])
					{
						tmp[8] = x + 4; tmp[9] = y;
					}
				}
				return true;
			}
			
			if (y < GRID_SIZE - 2 && grid[x][y] == grid[x][y + 1] && grid[x][y] == grid[x][y + 2])
			{
				tmp[0] = x; tmp[1] = x; tmp[2] = x;
				tmp[3] = y; tmp[4] = y + 1; tmp[5] = y + 2;
				if (y < GRID_SIZE - 3 && grid[x][y] == grid[x][y + 3])
				{
					tmp[6] = x; tmp[7] = y + 3;

					if (y < GRID_SIZE - 4 && grid[x][y] == grid[x][y + 4])
					{
						tmp[8] = x; tmp[9] = y + 4;
					}
				}
				return true;
			}
		}
	}
	return false;
}

void Grid::update_grid(vector<int>& pos)
{
	srand(time(NULL));

	int num = rand() % 5;

	if (pos[0] == pos[2])
	{
		int j = pos[0];
		while (j > 0)
		{
			grid[j][pos[3]] = grid[j - 1][pos[3]];
			grid[j][pos[4]] = grid[j - 1][pos[4]];
			grid[j][pos[5]] = grid[j - 1][pos[5]];
			if (pos[6] != -1)
			{
				grid[j][pos[7]] = grid[j - 1][pos[7]];
			}
			if (pos[8] != -1)
			{
				grid[j][pos[9]] = grid[j - 1][pos[9]];
			}
			j--;
		}
		grid[0][pos[3]] = ++num % 5;
		grid[0][pos[4]] = ++num % 5;
		grid[0][pos[5]] = ++num % 5;
		if (pos[6] != -1)
		{
			grid[0][pos[7]] = ++num % 5;
		}
		if (pos[8] != -1)
		{
			grid[0][pos[9]] = ++num % 5;
		}
	}
	else 
	{
		int j = pos[2];
		int x = 3;
		if (pos[6] != -1)
		{
			j = pos[6];
			x = 4;
			if (pos[8] != -1)
			{
				j = pos[8];
				x = 5;
			}
		}

		int column = pos[3];
		while (j > x - 1)
		{
			grid[j][column] = grid[j - x][column];
			j--;
		}
		//grid[0][column] = ++num % 5;
		//grid[1][column] = ++num % 5;
		//grid[2][column] = ++num % 5;
		for (int i = 0; i < x; i++)
		{
			grid[i][column] = ++num % 5;
		}
	}
}
