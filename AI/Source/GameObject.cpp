
#include "GameObject.h"

bool GameObject::Handle(Message* message)
{
	return false;
}

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f)
{
}

GameObject::~GameObject()
{
}