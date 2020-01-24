#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Maze.h"
#include <queue>
#include "StateMachine.h"
#include "ObjectBase.h"

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
		GO_MINE,
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
	StateMachine* sm;

	//GameObject Stats
	float health;
	float damage;

	//Turn
	bool turnOver;
	
	int inventorySize;
	std::vector<Maze::LOOT_TYPE> inventoryList;

	virtual bool Handle(Message* message);

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif