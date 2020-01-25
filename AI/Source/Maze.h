#ifndef MAZE_H
#define MAZE_H

#include <vector>

struct MazePt
{
	int x, y;
	MazePt(int _x = 0, int _y = 0) : x(_x), y(_y) {}
	void Set(int _x = 0, int _y = 0) {x = _x; y = _y;}
};

class Maze
{
public:
	enum TILE_CONTENT
	{
		TILE_MINE = -3,
		TILE_PLAYER = -2,
		TILE_WALL = -1,
		TILE_FOG = 0,
		TILE_EMPTY = 1,
		TILE_WATER = 2,
		TILE_MUD = 3
	};
	enum DIRECTION
	{
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
		DIR_TOP_RIGHT,
		DIR_TOP_LEFT,
		DIR_BOT_RIGHT,
		DIR_BOT_LEFT
	};
	enum LOOT_TYPE
	{
		LOOT_FOOD,
		LOOT_DRUG,
		LOOT_HPPACK,
		LOOT_DMGBOOST
	};
	struct Loot
	{
		Loot(int i, Maze::LOOT_TYPE type) : index(i), type(type) { }
		int index;
		Maze::LOOT_TYPE type;
	};
	Maze();
	~Maze();
	void Generate(unsigned key, unsigned size, MazePt start, float wallLoad);
	void GenerateLoot(float lootLoad);
	void GenerateTiles(float tileLoad);
	bool Move(DIRECTION direction);
	unsigned GetKey();
	unsigned GetSize();
	MazePt GetCurr();
	int GetNumMove();
	void SetCurr(MazePt newCurr);
	void SetNumMove(int num);
	void ReadMaze();

	TILE_CONTENT See(MazePt tile);
	std::vector<TILE_CONTENT> m_grid; //move from private
	std::vector<Loot*> m_loot;

private:
	//Do not make these public
	unsigned m_key;
	unsigned m_size;
	//std::vector<TILE_CONTENT> m_grid;
	MazePt m_curr;
	int m_numMove;
};

#endif