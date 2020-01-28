#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"

struct MessageFindIndex : public Message
{
	MessageFindIndex(GameObject* goValue) : go(goValue) {}
	virtual ~MessageFindIndex() {}

	GameObject* go;
};

struct MessageMoveType : public Message
{
	MessageMoveType(GameObject* goValue) : go(goValue) {}
	virtual ~MessageMoveType() {}

	GameObject* go;
};

struct MessageDetonate : public Message
{
	MessageDetonate(GameObject* goValue) : go(goValue) {}
	virtual ~MessageDetonate() {}

	GameObject* go;
};

#endif
