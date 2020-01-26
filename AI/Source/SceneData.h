#ifndef SCENE_DATA
#define SCENE_DATA

#include "Vector3.h"
#include "SingletonTemplate.h"
#include "GameObject.h"

class SceneData : public Singleton<SceneData>
{
	friend Singleton<SceneData>;
public:
	void SetGridNo(int no);
	int GetGridNo();
	void SetMyGrid(std::vector<Maze::TILE_CONTENT> grid);
	std::vector<Maze::TILE_CONTENT> GetMyGrid();
private:
	SceneData();
	~SceneData();

	int gridNo;
	std::vector<Maze::TILE_CONTENT> m_myGrid;
};

#endif
