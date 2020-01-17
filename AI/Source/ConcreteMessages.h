#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"

struct MessageFindNearest : public Message
{
	MessageFindNearest(GameObject *goValue) : go(goValue) {}
	virtual ~MessageFindNearest() {}

	GameObject* go;
};

struct MessageCheckActive : public Message
{
	enum SEARCH_TYPE
	{
		SEARCH_NONE = 0,
		CHECK_ACTIVE,
	};
	MessageCheckActive(SEARCH_TYPE typeValue) : type(typeValue) {}
	virtual ~MessageCheckActive() {}

	SEARCH_TYPE type;
};

struct MessageKillAll : public Message
{
	enum OBJ_TYPE
	{
		none = 0,
		tower,
		skeleton,
		witch,
		archer,
		giant,
		prince,
		giantskele,
		bomb,
		all,
	};
	MessageKillAll(OBJ_TYPE typeVal) : type(typeVal) {}
	virtual ~MessageKillAll() {}

	OBJ_TYPE type;
};

struct MessageSpawnUnits : public Message
{
	enum SPAWN_TYPE
	{
		SPAWN_SKELETON,
		SPAWN_BOMB,
	};
	MessageSpawnUnits(SPAWN_TYPE typeVal, GameObject *goVal) : type(typeVal), go(goVal) {}
	virtual ~MessageSpawnUnits() {}

	SPAWN_TYPE type;
	GameObject* go;
};

struct MessageAOEDamage : public Message
{
	MessageAOEDamage(float radVal, GameObject* goVal) : radius(radVal), go(goVal) {}
	virtual ~MessageAOEDamage() {}

	float radius;
	GameObject* go;
};

#endif
