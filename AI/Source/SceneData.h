#ifndef SCENE_DATA
#define SCENE_DATA

#include "Vector3.h"
#include "SingletonTemplate.h"
#include "GameObject.h"

class SceneData : public Singleton<SceneData>
{
	friend Singleton<SceneData>;
public:


private:
	SceneData();
	~SceneData();
};

#endif
