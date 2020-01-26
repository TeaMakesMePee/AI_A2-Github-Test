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
	PostOffice::GetInstance()->Send("Scene", new MessageFindIndex(m_go));
}

void StateIdle::Update(double dt)
{
	if (m_go->targetEnemy == NULL)
	{
		m_go->sm->SetNextState("Move");
	}
	else
	{
		bool found = false;
		for (int x = 0; x < m_go->adjIndexes.size(); ++x)
		{
			if ((m_go->targetEnemy->curr.y * SceneData::GetInstance()->GetGridNo() + m_go->targetEnemy->curr.x) == m_go->adjIndexes[x])
			{
				m_go->sm->SetNextState("Attack");
				found = true;
			}
		}

		if (!found)
		{
			for (int x = 0; x < m_go->visIndexes.size(); ++x)
			{
				for (int y = 0; y < m_go->targetEnemy->adjIndexes.size(); ++y)
				{
					if (m_go->visIndexes[x] == m_go->targetEnemy->adjIndexes[y] && SceneData::GetInstance()->GetMyGrid()[m_go->visIndexes[x]] > -1)
					{
						m_go->targetIndex = m_go->visIndexes[x];
						m_go->sm->SetNextState("Move");
					}
				}
			}
			m_go->targetEnemy->adjIndexes.clear();
		}
	}
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
	PostOffice::GetInstance()->Send("Scene", new MessageMoveType(m_go));
}

void StateMove::Update(double dt)
{
	if (m_go->targetEnemy)
	{
		for (int x = 0; x < m_go->adjIndexes.size(); ++x)
		{
			if (m_go->targetEnemy->curr.y * SceneData::GetInstance()->GetGridNo() + m_go->targetEnemy->curr.x == m_go->adjIndexes[x])
			{
				m_go->sm->SetNextState("Attack");
				m_go->path.clear();
			}
		}
	}
	else if (m_go->path.empty())
	{
		m_go->sm->SetNextState("Idle");
		m_go->turnOver = true;
	}
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
	m_go->targetEnemy->health -= m_go->damage;
	if (m_go->targetEnemy->health <= 0.f)
		m_go->targetEnemy->sm->SetNextState("Dead");
}

void StateAttack::Update(double dt)
{
	m_go->sm->SetNextState("Idle");
	m_go->turnOver = true;
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
	m_go->cdTimer = 2.0;
}

void StateDead::Update(double dt)
{
	m_go->cdTimer -= dt;
	if (m_go->cdTimer <= 0.0)
	{
		m_go->active = false;
		m_go->turnOver = true;
	}
}

void StateDead::Exit()
{
}