#ifndef SCENE_DATA
#define SCENE_DATA

#include "Vector3.h"
#include "SingletonTemplate.h"
#include "GameObject.h"

class SceneData : public Singleton<SceneData>
{
	friend Singleton<SceneData>;
public:
	enum WHO_CONTENT
	{
		WHO_NONE = 0,
		WHO_TOWER,
		WHO_VOID,
	};

	int GetObjectCount();
	void SetObjectCount(int objectCount);
	int GetFishCount();
	void SetFishCount(int fishCount);
	float GetGridSize();
	void SetGridSize(float gridsize);
	float GetGridOffset();
	void SetGridOffset(float gridoffset);
	int GetGridCount();
	void SetGridCount(int gridoffset);
	void SetGridOccupied(Vector3 pos, WHO_CONTENT type);
	void SetGridOccupied(int xCord, int yCord, WHO_CONTENT type);
	bool GetIfGridOccupied(Vector3 pos);
	Vector3 GetGridCenter(int xCord, int yCord);
	Vector3 GetGridCenter(int gridVal);
	float* GetPlayers(void);
	void SetPlayers(float* pyr);
	double* GetBuffers(void);
	void SetBuffers(double* bfrs);
	GameObject::GAMEOBJECT_TYPE GetNextBotPlayer(void);
	void SetNextBotPlayer(GameObject::GAMEOBJECT_TYPE plyr);
	GameObject::GAMEOBJECT_TYPE GetNextTopPlayer(void);
	void SetNextTopPlayer(GameObject::GAMEOBJECT_TYPE plyr);
	void SetGameOver(bool gameState);
	bool GetGameOver(void);
	void SetTopWinner(bool winner);
	bool GetTopWinner(void);
	void SetSimSpeed(float x);
	float GetSimSpeed(void);

private:
	SceneData();
	~SceneData();
	int m_objectCount;
	int m_fishCount;
	int m_noGrid;
	float m_gridSize;
	float m_gridOffset;
	WHO_CONTENT grid[400] = { SceneData::WHO_NONE };
	float players[2] = { 0.f, 0.f };
	double buffers[2] = { -1.0, -1.0 };
	GameObject::GAMEOBJECT_TYPE nextBottomPlayer;
	GameObject::GAMEOBJECT_TYPE nextTopPlayer;
	bool topWinner;
	bool gameOver;
	float m_speed;
};

#endif
