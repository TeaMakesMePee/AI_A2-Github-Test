#include "StatesGeneric.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

StateIdle::StateIdle(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateIdle::~StateIdle()
{
}

void StateIdle::Enter()
{

}

void StateIdle::Update(double dt)
{

}

void StateIdle::Exit()
{

}

StateMove::StateMove(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateMove::~StateMove()
{
}

void StateMove::Enter()
{

}

void StateMove::Update(double dt)
{

}

void StateMove::Exit()
{

}

StateAttack::StateAttack(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateAttack::~StateAttack()
{
}

void StateAttack::Enter()
{

}

void StateAttack::Update(double dt)
{

}

void StateAttack::Exit()
{

}

StateDead::StateDead(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateDead::~StateDead()
{
}

void StateDead::Enter()
{
	//Remember to remove GO from either botSide/topSide list when dead
}

void StateDead::Update(double dt)
{

}

void StateDead::Exit()
{
}