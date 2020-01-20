#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Maze.h"
#include <queue>

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CROSS,
		GO_CIRCLE,
		GO_GIANTSOLDIER,
		GO_SOLDIER,
		GO_K9,
		GO_TOTAL, //must be last
	};


	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;
	Vector3 target;
	int id;
	int steps;
	std::vector<Maze::TILE_CONTENT> grid;
	std::vector<bool> visited;
	std::vector<float> aGlobal;
	std::vector<float> aLocal;
	std::vector<MazePt> stack;
	std::vector<MazePt> path;
	std::queue<MazePt> m_queue;
	std::vector<MazePt> previous;

	MazePt curr;

	double timedelay;

	int visRadius;
	std::vector<int> visIndexes;

	//GameObject Stats
	float health;
	float damage;
	float speed;
	
	int inventorySize;
	std::vector<Maze::LOOT_TYPE> inventoryList;

	GameObject(int size, GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif