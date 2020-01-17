#ifndef SCENE_MAZE_H
#define SCENE_MAZE_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Maze.h"
#include <queue>

class SceneMaze : public SceneBase
{
public:
	SceneMaze();
	~SceneMaze();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	GameObject* FetchGO();

	void DFS(MazePt curr);
	bool BFS(MazePt start, MazePt end);

protected:

	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	int m_noGrid;
	float m_gridSize;
	float m_gridOffset;

	Maze m_maze;
	MazePt m_start;
	MazePt m_end;
	MazePt m_curr;
	std::vector<Maze::TILE_CONTENT> m_myGrid;
	std::vector<bool> m_visited; //visited set for DFS/BFS
	std::queue<MazePt> m_queue; //queue for BFS
	std::vector<MazePt> m_previous; //to store previous tile
	std::vector<MazePt> m_shortestPath; //to store shortest path
	unsigned m_mazeKey;
};

#endif