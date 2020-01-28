#include "Maze.h"
#include <iostream>
#include "MyMath.h"
#include "Application.h"
#include <fstream>
#include <string>

Maze::Maze()
{
}

Maze::~Maze()
{
}

void Maze::Generate(unsigned key, unsigned size, MazePt start, float wallLoad)
{
	m_grid.clear();
	if (size == 0)
		return;
	start.x = Math::Clamp(start.x, 0, (int)size - 1);
	start.y = Math::Clamp(start.y, 0, (int)size - 1);
	wallLoad = Math::Clamp(wallLoad, 0.f, 0.8f);
	unsigned total = size * size;
	m_grid.resize(total);
	std::fill(m_grid.begin(), m_grid.end(), TILE_EMPTY);
	unsigned startId = start.y * size + start.x;
	srand(time(NULL));
	//for (int i = 0; i < (int)total * wallLoad;)
	//{
	//	unsigned chosen = rand() % total;
	//	if (chosen == startId)
	//		continue;
	//	if (m_grid[chosen] == TILE_EMPTY)
	//	{
	//		//m_grid[chosen] = TILE_WALL;
	//		m_grid[chosen] = TILE_EMPTY;
	//		++i;
	//	}
	//}
	////std::cout << "Maze " << key << std::endl;
	//for (int row = (int)size - 1; row >= 0; --row)
	//{
	//	for (int col = 0; col < (int)size; ++col)
	//	{
	//		if (m_grid[row * size + col] == TILE_WALL)
	//			std::cout << "1 ";
	//		else
	//			std::cout << "0 ";
	//	}
	//	std::cout << std::endl;
	//}
	m_key = key;
	m_size = size;
	m_numMove = 0;
	ReadMaze();
}

void Maze::GenerateLoot(float lootLoad)
{
	srand(time(NULL));
	for (int i = 0; i < (int)m_grid.size() * lootLoad;)
	{
		unsigned chosen = rand() % (int)m_grid.size();
		if (m_grid[chosen] == TILE_EMPTY)
		{
			int percent = Math::RandIntMinMax(0, 100);
			Loot* loot;
			if (percent <= 15)
				loot = new Loot(chosen, LOOT_TYPE::LOOT_HPPACK, 3);
			else if (percent <= 30)
				loot = new Loot(chosen, LOOT_TYPE::LOOT_DMGBOOST, 3);
			else if (percent <= 65)
				loot = new Loot(chosen, LOOT_TYPE::LOOT_DRUG, 2);
			else if (percent <= 100)
				loot = new Loot(chosen, LOOT_TYPE::LOOT_FOOD, 2);
			m_loot.push_back(loot);
			++i;
		}
	}
}

void Maze::GenerateTiles(float tileLoad)
{
	srand(time(NULL));
	for (int i = 0; i < (int)m_grid.size() * tileLoad;)
	{
		unsigned chosen = rand() % (int)m_grid.size();
		if (m_grid[chosen] == TILE_EMPTY)
		{
			int percent = Math::RandIntMinMax(0, 100);
			if (percent <= 30)
				m_grid[chosen] = TILE_MUD;
			else if (percent <= 100)
				m_grid[chosen] = TILE_WATER;
			++i;
		}
	}
}

unsigned Maze::GetKey()
{
	return m_key;
}

unsigned Maze::GetSize()
{
	return m_size;
}

MazePt Maze::GetCurr()
{
	return m_curr;
}

int Maze::GetNumMove()
{
	return m_numMove;
}

void Maze::SetCurr(MazePt newCurr)
{
	//Application::GetInstance().Iterate();
	//Sleep(5000);
	++m_numMove;
	m_curr = newCurr;
}

void Maze::SetNumMove(int num)
{
	m_numMove = num;
}

void Maze::ReadMaze()
{
	std::string line, file;
	int rand = Math::RandIntMinMax(1, 3);
	if (rand == 1)
		file = "Map1.txt";
	else if (rand == 2)
		file = "Map2.txt";
	else if (rand == 3)
		file = "Map3.txt";
	std::ifstream myfile(file);
	int row = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int x = 0; x < line.size(); ++x)
			{
				if (line[x] == '0')
					m_grid[row * m_size + x] = Maze::TILE_EMPTY;
				else if (line[x] == '1')
					m_grid[row * m_size + x] = Maze::TILE_WALL;
			}
			row++;
		}
		myfile.close();
	}
}

Maze::TILE_CONTENT Maze::See(MazePt tile)
{
	int index = tile.y * m_size + tile.x;
	return m_grid[index];
}

bool Maze::Move(DIRECTION direction)
{
	//Application::GetInstance().Iterate();
	//Sleep(10);
	++m_numMove;
	MazePt temp = m_curr;
	switch (direction)
	{
	case DIR_LEFT:
		if (temp.x == 0)
			return false;
		temp.x -= 1;
		break;
	case DIR_RIGHT:
		if (temp.x == (int)m_size - 1)
			return false;
		temp.x += 1;
		break;
	case DIR_UP:
		if (temp.y == (int)m_size - 1)
			return false;
		temp.y += 1;
		break;
	case DIR_DOWN:
		if (temp.y == 0)
			return false;
		temp.y -= 1;
		break;
	case DIR_TOP_RIGHT:
		if (temp.y == (int)m_size - 1)
			return false;
		if (temp.x == (int)m_size - 1)
			return false;
		temp.y += 1;
		temp.x += 1;
		break;
	case DIR_TOP_LEFT:
		if (temp.y == (int)m_size - 1)
			return false;
		if (temp.x == 0)
			return false;
		temp.y += 1;
		temp.x -= 1;
		break;
	case DIR_BOT_RIGHT:
		if (temp.y == 0)
			return false;
		if (temp.x == (int)m_size - 1)
			return false;
		temp.y -= 1;
		temp.x += 1;
		break;
	case DIR_BOT_LEFT:
		if (temp.y == 0)
			return false;
		if (temp.x == 0)
			return false;
		temp.y -= 1;
		temp.x -= 1;
		break;
	}
	int tempId = temp.y * m_size + temp.x;
	if (m_grid[tempId] == TILE_WALL)
		return false;
	m_curr = temp;
	return true;
}
