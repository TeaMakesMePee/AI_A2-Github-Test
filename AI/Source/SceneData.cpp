#include "SceneData.h"

int SceneData::GetObjectCount()
{
	return m_objectCount;
}

void SceneData::SetObjectCount(int objectCount)
{
	m_objectCount = objectCount;
}

int SceneData::GetFishCount()
{
	return m_fishCount;
}

void SceneData::SetFishCount(int fishCount)
{
	m_fishCount = fishCount;
}

float SceneData::GetGridSize()
{
	return m_gridSize;
}

void SceneData::SetGridSize(float gridsize)
{
	this->m_gridSize = gridsize;
}

float SceneData::GetGridOffset()
{
	return m_gridOffset;
}

void SceneData::SetGridOffset(float gridoffset)
{
	this->m_gridOffset = gridoffset;
}

int SceneData::GetGridCount()
{
	return m_noGrid;
}

void SceneData::SetGridCount(int gridoffset)
{
	this->m_noGrid = gridoffset;
}

void SceneData::SetGridOccupied(Vector3 pos, WHO_CONTENT type)
{
	int gridX = pos.x / m_gridSize;
	int gridY = pos.y / m_gridSize;
	int m_gridVal = gridX + gridY * m_noGrid;
	grid[m_gridVal] = type;
}

void SceneData::SetGridOccupied(int xCord, int yCord, WHO_CONTENT type)
{
	int m_gridVal = xCord + yCord * m_noGrid;
	grid[m_gridVal] = type;
}

bool SceneData::GetIfGridOccupied(Vector3 pos)
{
	int gridX = pos.x / m_gridSize;
	int gridY = pos.y / m_gridSize;
	int m_gridVal = gridX + gridY * m_noGrid;
	if (grid[m_gridVal] == WHO_NONE)
		return false;
	else
		return true;
}

Vector3 SceneData::GetGridCenter(int xCord, int yCord)
{
	return Vector3(m_gridSize * xCord + m_gridOffset, m_gridSize * yCord + m_gridOffset);
}

Vector3 SceneData::GetGridCenter(int gridVal)
{
	int y = gridVal / 3;
	int x = gridVal - y * 3;
	return Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
}

float* SceneData::GetPlayers(void)
{
	return players;
}

void SceneData::SetPlayers(float* pyr)
{
	for (int x = 0; x < 2; ++x)
		players[x] = pyr[x];
}

double* SceneData::GetBuffers(void)
{
	return buffers;
}

void SceneData::SetBuffers(double* bfrs)
{
	for (int x = 0; x < 2; ++x)
		buffers[x] = bfrs[x];
}

GameObject::GAMEOBJECT_TYPE SceneData::GetNextBotPlayer(void)
{
	return nextBottomPlayer;
}

void SceneData::SetNextBotPlayer(GameObject::GAMEOBJECT_TYPE plyr)
{
	this->nextBottomPlayer = plyr;
}

GameObject::GAMEOBJECT_TYPE SceneData::GetNextTopPlayer(void)
{
	return nextTopPlayer;
}

void SceneData::SetNextTopPlayer(GameObject::GAMEOBJECT_TYPE plyr)
{
	this->nextTopPlayer = plyr;
}

void SceneData::SetGameOver(bool gameState)
{
	this->gameOver = gameState;
}

bool SceneData::GetGameOver(void)
{
	return this->gameOver;
}

void SceneData::SetTopWinner(bool winner)
{
	this->topWinner = winner;
}

bool SceneData::GetTopWinner(void)
{
	return this->topWinner;
}

void SceneData::SetSimSpeed(float x)
{
	this->m_speed = x;
}

float SceneData::GetSimSpeed(void)
{
	return this->m_speed;
}

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}
