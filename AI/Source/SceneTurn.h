#ifndef SCENE_TURN_H
#define SCENE_TURN_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Maze.h"
#include <queue>

class SceneTurn : public SceneBase
{
public:
	SceneTurn();
	~SceneTurn();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	void RenderLoot(int index, Maze::LOOT_TYPE type);
	void UpdateVisibleTiles(GameObject* go, MazePt point, int visRadius);

	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);

	void DFS(MazePt curr);
	bool BFS(MazePt start, MazePt end);

	void DFSOnce(GameObject* go);
	bool BFSLimit(GameObject* go, MazePt end, int limit);

	bool AStar(GameObject* go, MazePt end);

	void SetUnitStats(GameObject* go);
	
	//Event related
	void GenerateEventBombs();
protected:

	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	int m_noGrid;
	float m_gridSize;
	float m_gridOffset;
	float m_rightOffset;

	Maze m_maze;
	MazePt m_start;
	MazePt m_end;
	std::vector<Maze::TILE_CONTENT> m_myGrid; //read maze and store here
	std::vector<bool> m_visited; //visited set for DFS/BFS
	std::queue<MazePt> m_queue; //queue for BFS
	std::vector<MazePt> m_previous; //to store previous tile
	std::vector<MazePt> m_shortestPath;  //to store shortest path
	std::vector<bool> m_visible; //visible tiles
	unsigned m_mazeKey;
	float m_wallLoad;
	float appliedXScale;

	int m_turn;
	double timer;

	GameObject* target;

	//To keep track of whose turn
	bool botsideTurn;

	//List of bottom side players
	std::vector<GameObject*> botsideList;

	//List of top side players
	std::vector<GameObject*> topsideList;

	//To keep track of elapsed time
	double elapsedTime;

	//Event related
	double eventTime;
	int lifeinTurns;
	bool eventActive;
	std::vector<GameObject*> mineList;
};

#endif