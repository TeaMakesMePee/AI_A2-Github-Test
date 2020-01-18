
#include "GameObject.h"

GameObject::GameObject(int size, GAMEOBJECT_TYPE typeValue)
	: inventorySize(size),
	type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f)
{
}

GameObject::~GameObject()
{
}