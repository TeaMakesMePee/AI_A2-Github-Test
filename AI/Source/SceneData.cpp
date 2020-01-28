#include "SceneData.h"

void SceneData::SetGridNo(int no)
{
	this->gridNo = no;
}

int SceneData::GetGridNo()
{
	return this->gridNo;
}

void SceneData::SetMyGrid(std::vector<Maze::TILE_CONTENT> grid)
{
	this->m_myGrid = grid;
}

std::vector<Maze::TILE_CONTENT> SceneData::GetMyGrid()
{
	return m_myGrid;
}

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}
