#ifndef STATES_ALL_UNITS_H
#define STATES_ALL_UNITS_H

#include "State.h"
#include "GameObject.h"

class StateIdle : public State
{
	GameObject* m_go;
public:
	StateIdle(const std::string& stateID, GameObject* go);
	virtual ~StateIdle();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateMove : public State
{
	GameObject* m_go;
public:
	StateMove(const std::string& stateID, GameObject* go);
	virtual ~StateMove();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateAttack : public State
{
	GameObject* m_go;
public:
	StateAttack(const std::string& stateID, GameObject* go);
	virtual ~StateAttack();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateDead : public State
{
	GameObject* m_go;
public:
	StateDead(const std::string& stateID, GameObject* go);
	virtual ~StateDead();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif