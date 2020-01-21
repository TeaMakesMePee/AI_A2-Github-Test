#include "SceneTurn.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include <list>

#include "StateMachine.h"
#include "StatesGeneric.h"

SceneTurn::SceneTurn()
{
}

SceneTurn::~SceneTurn()
{
}

void SceneTurn::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	appliedXScale = 1.2f;

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_noGrid = 20;
	m_gridSize = m_worldHeight / (m_noGrid + 1);
	m_gridOffset = m_gridSize / 2;

	timer = 0.0;
	m_start.Set(0, 0);
	m_mazeKey = 0;
	m_wallLoad = 0.3f;
	m_maze.Generate(m_mazeKey, m_noGrid, m_start, m_wallLoad); //Generate new maze
	m_maze.GenerateTiles(0.1f);
	m_maze.GenerateLoot(0.045f);
	m_myGrid.resize(m_noGrid * m_noGrid);
	m_visited.resize(m_noGrid * m_noGrid);
	m_visible.resize(m_noGrid * m_noGrid);
	m_previous.resize(m_noGrid * m_noGrid);
	std::fill(m_myGrid.begin(), m_myGrid.end(), Maze::TILE_FOG);
	std::fill(m_visited.begin(), m_visited.end(), false);
	std::fill(m_visible.begin(), m_visible.end(), false);
	m_myGrid[m_start.y * m_noGrid + m_start.x] = Maze::TILE_EMPTY;
	DFS(m_start);
	m_rightOffset = 48.f;
	botsideTurn = true;
	//im just testing out some stuff


	//Init event variables
	eventTime = static_cast<double>(Math::RandFloatMinMax(30.f, 60.f));
	eventActive = false;
	lifeinTurns = 5;
}

GameObject* SceneTurn::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active && go->type == type)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 5; ++i)
	{
		GameObject* go = new GameObject(type);
		m_goList.push_back(go);
		if (type == GameObject::GO_K9)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateIdle("Idle", go));
			go->sm->AddState(new StateMove("Move", go));
			go->sm->AddState(new StateAttack("Attack", go));
			go->sm->AddState(new StateDead("Dead", go));
		}
	}
	return FetchGO(type);
}

void SceneTurn::DFS(MazePt curr)
{
	m_visited[curr.y * m_noGrid + curr.x] = true;

	//UP
	if(curr.y < m_noGrid - 1)
	{
		MazePt next(curr.x, curr.y + 1);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_UP) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = m_maze.m_grid[next.y * m_noGrid + next.x];
				DFS(next);
				m_maze.Move(Maze::DIR_DOWN);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}

	//DOWN
	if (curr.y > 0)
	{
		MazePt next(curr.x, curr.y - 1);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_DOWN) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = m_maze.m_grid[next.y * m_noGrid + next.x];
				DFS(next);
				m_maze.Move(Maze::DIR_UP);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}

	//LEFT
	if (curr.x > 0)
	{
		MazePt next(curr.x - 1, curr.y);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_LEFT) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = m_maze.m_grid[next.y * m_noGrid + next.x];
				DFS(next);
				m_maze.Move(Maze::DIR_RIGHT);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}

	//RIGHT 
	if (curr.x < m_noGrid - 1)
	{
		MazePt next(curr.x + 1, curr.y);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_RIGHT) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = m_maze.m_grid[next.y * m_noGrid + next.x];
				DFS(next);
				m_maze.Move(Maze::DIR_LEFT);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}

	if (curr.x % 2) //if odd x 
	{
		//TOP LEFT 
		if (curr.x - 1 > -1 && curr.y + 1 < m_noGrid)
		{
			MazePt next(curr.x - 1, curr.y + 1);
			if (!m_visited[next.y * m_noGrid + next.x])
			{
				if (m_maze.Move(Maze::DIR_TOP_LEFT) == true)
				{
					m_myGrid[next.y * m_noGrid + next.x] = m_maze.m_grid[next.y * m_noGrid + next.x];
					DFS(next);
					m_maze.Move(Maze::DIR_BOT_RIGHT);
				}
				else
				{
					m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
				}
			}
		}

		//TOP RIGHT 
		if (curr.x + 1 < m_noGrid && curr.y + 1 < m_noGrid)
		{
			MazePt next(curr.x + 1, curr.y + 1);
			if (!m_visited[next.y * m_noGrid + next.x])
			{
				if (m_maze.Move(Maze::DIR_TOP_RIGHT) == true)
				{
					m_myGrid[next.y * m_noGrid + next.x] = m_maze.m_grid[next.y * m_noGrid + next.x];
					DFS(next);
					m_maze.Move(Maze::DIR_BOT_LEFT);
				}
				else
				{
					m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
				}
			}
		}
	}
	else //even x
	{
		//BOT LEFT 
		if (curr.x - 1 > -1 && curr.y - 1 > -1)
		{
			MazePt next(curr.x - 1, curr.y - 1);
			if (!m_visited[next.y * m_noGrid + next.x])
			{
				if (m_maze.Move(Maze::DIR_BOT_LEFT) == true)
				{
					m_myGrid[next.y * m_noGrid + next.x] = m_maze.m_grid[next.y * m_noGrid + next.x];
					DFS(next);
					m_maze.Move(Maze::DIR_TOP_RIGHT);
				}
				else
				{
					m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
				}
			}
		}

		//BOT RIGHT 
		if (curr.x + 1 < m_noGrid && curr.y - 1 > -1)
		{
			MazePt next(curr.x + 1, curr.y - 1);
			if (!m_visited[next.y * m_noGrid + next.x])
			{
				if (m_maze.Move(Maze::DIR_BOT_RIGHT) == true)
				{
					m_myGrid[next.y * m_noGrid + next.x] = m_maze.m_grid[next.y * m_noGrid + next.x];
					DFS(next);
					m_maze.Move(Maze::DIR_TOP_LEFT);
				}
				else
				{
					m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
				}
			}
		}
	}
}

bool SceneTurn::BFS(MazePt start, MazePt end)
{
	std::fill(m_visited.begin(), m_visited.end(), false);
	while (!m_queue.empty())
		m_queue.pop();
	m_shortestPath.clear();
	m_queue.push(start);
	m_maze.SetNumMove(0);
	while (!m_queue.empty())
	{
		MazePt curr = m_queue.front();
		m_maze.SetCurr(curr);
		m_queue.pop();
		if (curr.x == end.x && curr.y == end.y)
		{
			while (!(curr.x == start.x && curr.y == start.y))
			{
				m_shortestPath.insert(m_shortestPath.begin(), curr);
				curr = m_previous[curr.y * m_noGrid + curr.x];
			}
			m_shortestPath.insert(m_shortestPath.begin(), curr);
			return true;
		}
		//UP
		if (curr.y < m_noGrid - 1)
		{
			MazePt next(curr.x, curr.y + 1);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//DOWN
		if (curr.y > 0)
		{
			MazePt next(curr.x, curr.y - 1);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//LEFT
		if (curr.x > 0)
		{
			MazePt next(curr.x - 1, curr.y);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//RIGHT
		if (curr.x < m_noGrid - 1)
		{
			MazePt next(curr.x + 1, curr.y);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
	}
	return false;
}

void SceneTurn::DFSOnce(GameObject* go)
{
	go->stack.push_back(go->curr);
	go->visited[go->curr.y * m_noGrid + go->curr.x] = true;

	//check if up is visited
	if (go->curr.y + 1 < m_noGrid)
	{
		if (!go->visited[(go->curr.y + 1) * m_noGrid + go->curr.x])
		{
			go->grid[(go->curr.y + 1) * m_noGrid + go->curr.x] = m_maze.See(MazePt(go->curr.x, go->curr.y + 1));
			if (go->grid[(go->curr.y + 1) * m_noGrid + go->curr.x] == Maze::TILE_EMPTY)
			{
				go->curr.y += 1;
				return;
			}
		}
	}

	//check if left is visited
	if (go->curr.x - 1 > -1)
	{
		if (!go->visited[go->curr.y * m_noGrid + go->curr.x - 1])
		{
			go->grid[go->curr.y * m_noGrid + go->curr.x - 1] = m_maze.See(MazePt(go->curr.x - 1, go->curr.y));
			if (go->grid[go->curr.y * m_noGrid + go->curr.x - 1] == Maze::TILE_EMPTY)
			{
				go->curr.x -= 1;
				return;
			}
		}
	}

	//check if right is visited
	if (go->curr.x + 1 < m_noGrid)
	{
		if (!go->visited[go->curr.y * m_noGrid + go->curr.x + 1])
		{
			go->grid[go->curr.y * m_noGrid + go->curr.x + 1] = m_maze.See(MazePt(go->curr.x + 1, go->curr.y));
			if (go->grid[go->curr.y * m_noGrid + go->curr.x + 1] == Maze::TILE_EMPTY)
			{
				go->curr.x += 1;
				return;
			}
		}
	}

	if (go->curr.x % 2) //if odd x 
	{
		//TOP LEFT 
		if (go->curr.x - 1 > -1 && go->curr.y + 1 < m_noGrid)
		{
			//MazePt next(curr.x - 1, curr.y + 1);
			if (!go->visited[(go->curr.y + 1) * m_noGrid + go->curr.x - 1])
			{
				go->grid[(go->curr.y + 1) * m_noGrid + go->curr.x - 1] = m_maze.See(MazePt(go->curr.x - 1, go->curr.y + 1));
				if (go->grid[(go->curr.y + 1) * m_noGrid + go->curr.x - 1] == Maze::TILE_EMPTY)
				{
					go->curr.x -= 1;
					go->curr.y += 1;
					return;
				}
			}
		}

		//TOP RIGHT 
		if (go->curr.x + 1 < m_noGrid && go->curr.y + 1 < m_noGrid)
		{
			//MazePt next(curr.x - 1, curr.y + 1);
			if (!go->visited[(go->curr.y + 1) * m_noGrid + go->curr.x + 1])
			{
				go->grid[(go->curr.y + 1) * m_noGrid + go->curr.x + 1] = m_maze.See(MazePt(go->curr.x + 1, go->curr.y + 1));
				if (go->grid[(go->curr.y + 1) * m_noGrid + go->curr.x + 1] == Maze::TILE_EMPTY)
				{
					go->curr.x += 1;
					go->curr.y += 1;
					return;
				}
			}
		}
	}
	else //even x
	{
		//BOT LEFT 
		if (go->curr.x - 1 > -1 && go->curr.y - 1 > -1)
		{
			//MazePt next(curr.x - 1, curr.y + 1);
			if (!go->visited[(go->curr.y - 1) * m_noGrid + go->curr.x - 1])
			{
				go->grid[(go->curr.y - 1) * m_noGrid + go->curr.x - 1] = m_maze.See(MazePt(go->curr.x - 1, go->curr.y - 1));
				if (go->grid[(go->curr.y - 1) * m_noGrid + go->curr.x - 1] == Maze::TILE_EMPTY)
				{
					go->curr.x -= 1;
					go->curr.y -= 1;
					return;
				}
			}
		}

		//BOT RIGHT 
		if (go->curr.x + 1 < m_noGrid && go->curr.y - 1 > -1)
		{
			//MazePt next(curr.x - 1, curr.y + 1);
			if (!go->visited[(go->curr.y - 1) * m_noGrid + go->curr.x + 1])
			{
				go->grid[(go->curr.y - 1) * m_noGrid + go->curr.x + 1] = m_maze.See(MazePt(go->curr.x + 1, go->curr.y - 1));
				if (go->grid[(go->curr.y - 1) * m_noGrid + go->curr.x + 1] == Maze::TILE_EMPTY)
				{
					go->curr.x += 1;
					go->curr.y -= 1;
					return;
				}
			}
		}
	}

	//check if down is visited
	if (go->curr.y - 1 > -1)
	{
		if (!go->visited[(go->curr.y - 1) * m_noGrid + go->curr.x])
		{
			go->grid[(go->curr.y - 1) * m_noGrid + go->curr.x] = m_maze.See(MazePt(go->curr.x, go->curr.y - 1));
			if (go->grid[(go->curr.y - 1) * m_noGrid + go->curr.x] == Maze::TILE_EMPTY)
			{
				go->curr.y -= 1;
				return;
			}
		}
	}

	go->stack.pop_back(); //clears stack
	if (!go->stack.empty())
	{
		go->curr = go->stack.back(); //set the backtrack
		go->stack.pop_back();
	}
}

bool SceneTurn::BFSLimit(GameObject* go, MazePt end, int limit)
{
	m_shortestPath.clear();
	std::fill(go->visited.begin(), go->visited.end(), false);
	while (!go->m_queue.empty())
		go->m_queue.pop();
	go->path.clear();
	go->previous.clear();
	go->previous.resize(m_noGrid * m_noGrid);
	go->m_queue.push(go->curr);
	float nearestDistance = FLT_MAX;
	MazePt nearest, curr, start = go->curr;
	go->visited[go->curr.y * m_noGrid + go->curr.x] = true;
	int loop = 0;
	while (!go->m_queue.empty() && loop < limit)
	{
		++loop;
		curr = go->m_queue.front();
		go->m_queue.pop();
		float distance = (Vector3(end.x, end.y, 0) - Vector3(curr.x, curr.y, 0)).Length();
		if (distance < nearestDistance)
		{
			nearestDistance = distance;
			nearest = curr;
		}
		if (curr.x == end.x && curr.y == end.y)
		{
			while (!(curr.x == start.x && curr.y == start.y))
			{
				go->path.insert(go->path.begin(), curr);
				curr = go->previous[curr.y * m_noGrid + curr.x];
			}
			go->path.insert(go->path.begin(), curr);
			std::cout << "Loops: " << loop << std::endl;
			return true;
		}
		//U,D,L,R
		if (curr.y < m_noGrid - 1)
		{
			MazePt next(curr.x, curr.y + 1);
			if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				go->previous[next.y * m_noGrid + next.x] = curr;
				go->m_queue.push(next);
				go->visited[next.y * m_noGrid + next.x] = true;
			}
		}
		if (curr.y > 0)
		{
			MazePt next(curr.x, curr.y - 1);
			if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				go->previous[next.y * m_noGrid + next.x] = curr;
				go->m_queue.push(next);
				go->visited[next.y * m_noGrid + next.x] = true;
			}
		}
		if (curr.x > 0)
		{
			MazePt next(curr.x - 1, curr.y);
			if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				go->previous[next.y * m_noGrid + next.x] = curr;
				go->m_queue.push(next);
				go->visited[next.y * m_noGrid + next.x] = true;
			}
		}
		if (curr.x < m_noGrid - 1)
		{
			MazePt next(curr.x + 1, curr.y);
			if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				go->previous[next.y * m_noGrid + next.x] = curr;
				go->m_queue.push(next);
				go->visited[next.y * m_noGrid + next.x] = true;
			}
		}
		if (curr.x % 2) //if odd x 
		{
			//TOP LEFT 
			if (curr.x - 1 > -1 && curr.y + 1 < m_noGrid)
			{
				MazePt next(curr.x - 1, curr.y + 1);
				if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
				{
					go->previous[next.y * m_noGrid + next.x] = curr;
					go->m_queue.push(next);
					go->visited[next.y * m_noGrid + next.x] = true;
				}
			}

			//TOP RIGHT 
			if (curr.x + 1 < m_noGrid && curr.y + 1 < m_noGrid)
			{
				MazePt next(curr.x + 1, curr.y + 1);
				if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
				{
					go->previous[next.y * m_noGrid + next.x] = curr;
					go->m_queue.push(next);
					go->visited[next.y * m_noGrid + next.x] = true;
				}
			}
		}
		else //even x
		{
			//BOT LEFT 
			if (curr.x - 1 > -1 && curr.y - 1 > -1)
			{
				MazePt next(curr.x - 1, curr.y - 1);
				if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
				{
					go->previous[next.y * m_noGrid + next.x] = curr;
					go->m_queue.push(next);
					go->visited[next.y * m_noGrid + next.x] = true;
				}
			}

			//BOT RIGHT 
			if (curr.x + 1 < m_noGrid && curr.y - 1 > -1)
			{
				MazePt next(curr.x + 1, curr.y - 1);
				if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
				{
					go->previous[next.y * m_noGrid + next.x] = curr;
					go->m_queue.push(next);
					go->visited[next.y * m_noGrid + next.x] = true;
				}
			}
		}
	}
	curr = nearest;
	while (!(curr.x == start.x && curr.y == start.y))
	{
		go->path.insert(go->path.begin(), curr);
		curr = go->previous[curr.y * m_noGrid + curr.x];
	}
	go->path.insert(go->path.begin(), curr);
	return false;
}

bool SceneTurn::AStar(GameObject* go, MazePt end)
{
	std::fill(go->visited.begin(), go->visited.end(), false);
	std::fill(go->aGlobal.begin(), go->aGlobal.end(), INFINITY);
	std::fill(go->aLocal.begin(), go->aLocal.end(), INFINITY);
	go->previous.clear();
	go->previous.resize(m_noGrid * m_noGrid);
	go->path.clear();
	
	MazePt start, curr = go->curr;
	go->aLocal[curr.y * m_noGrid + curr.x] = 1.f;

	float endxPos = m_gridSize * appliedXScale * end.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
	float endyPos = (!(end.x % 2) ? m_gridSize * end.y + m_gridOffset : m_gridSize * end.y + m_gridOffset + m_gridSize * 0.5f);

	float currxPos = m_gridSize * appliedXScale * curr.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
	float curryPos = (!(curr.x % 2) ? m_gridSize * curr.y + m_gridOffset : m_gridSize * curr.y + m_gridOffset + m_gridSize * 0.5f);

	go->aGlobal[curr.y * m_noGrid + curr.x] = (Vector3(endxPos, endyPos, 0.f) - Vector3(currxPos, curryPos, 0.f)).Length();

	std::vector<MazePt> unvisited;
	unvisited.push_back(curr);

	while (!unvisited.empty())
	{
		for (int iter = 1; iter < unvisited.size(); iter++)
		{
			for (int index = 0; index < unvisited.size() - iter; index++)
			{
				if (go->aGlobal[unvisited[index].y * m_noGrid + unvisited[index].x] > go->aGlobal[unvisited[index + 1].y * m_noGrid + unvisited[index + 1].x])
				{
					MazePt temp = unvisited[index];
					unvisited[index] = unvisited[index + 1];
					unvisited[index + 1] = temp;
				}
			}
		}

		while (!unvisited.empty() && go->visited[unvisited.front().y * m_noGrid + unvisited.front().x])
			unvisited.erase(unvisited.begin());

		if (unvisited.empty())
			break;

		curr = unvisited.front();
		
		//test
		if (curr.x == end.x && curr.y == end.y)
		{
			while (!(curr.x == start.x && curr.y == start.y))
			{
				go->path.insert(go->path.begin(), curr);
				curr = go->previous[curr.y * m_noGrid + curr.x];
			}
			go->path.insert(go->path.begin(), curr);
			return true;
		}

		go->visited[curr.y * m_noGrid + curr.x] = true;

		currxPos = m_gridSize * appliedXScale * curr.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
		curryPos = (!(curr.x % 2) ? m_gridSize * curr.y + m_gridOffset : m_gridSize * curr.y + m_gridOffset + m_gridSize * 0.5f);

		//U,D,L,R
		if (curr.y < m_noGrid - 1)
		{
			MazePt next(curr.x, curr.y + 1);
			if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] > -1)
			{
				unvisited.push_back(next);

				float nextxPos = m_gridSize * appliedXScale * next.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
				float nextyPos = (!(next.x % 2) ? m_gridSize * next.y + m_gridOffset : m_gridSize * next.y + m_gridOffset + m_gridSize * 0.5f);

				float nextLower = go->aLocal[curr.y * m_noGrid + curr.x] + m_myGrid[next.y * m_noGrid + next.x] * (Vector3(nextxPos, nextyPos, 0.f) - Vector3(currxPos, curryPos, 0.f)).Length();

				if (nextLower < go->aLocal[next.y * m_noGrid + next.x])
				{
					go->previous[next.y * m_noGrid + next.x] = curr;
					go->aLocal[next.y * m_noGrid + next.x] = nextLower;
					go->aGlobal[next.y * m_noGrid + next.x] = go->aLocal[next.y * m_noGrid + next.x] + (Vector3(endxPos, endyPos, 0.f) - Vector3(nextxPos, nextyPos, 0.f)).Length();
				}
			}
		}
		if (curr.y > 0)
		{
			MazePt next(curr.x, curr.y - 1);
			if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] > -1)
			{
				unvisited.push_back(next);

				float nextxPos = m_gridSize * appliedXScale * next.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
				float nextyPos = (!(next.x % 2) ? m_gridSize * next.y + m_gridOffset : m_gridSize * next.y + m_gridOffset + m_gridSize * 0.5f);

				float nextLower = go->aLocal[curr.y * m_noGrid + curr.x] + m_myGrid[next.y * m_noGrid + next.x] * (Vector3(nextxPos, nextyPos, 0.f) - Vector3(currxPos, curryPos, 0.f)).Length();

				if (nextLower < go->aLocal[next.y * m_noGrid + next.x])
				{
					go->previous[next.y * m_noGrid + next.x] = curr;
					go->aLocal[next.y * m_noGrid + next.x] = nextLower;
					go->aGlobal[next.y * m_noGrid + next.x] = go->aLocal[next.y * m_noGrid + next.x] + (Vector3(endxPos, endyPos, 0.f) - Vector3(nextxPos, nextyPos, 0.f)).Length();
				}
			}
		}
		if (curr.x > 0)
		{
			MazePt next(curr.x - 1, curr.y);
			if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] > -1)
			{
				unvisited.push_back(next);

				float nextxPos = m_gridSize * appliedXScale * next.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
				float nextyPos = (!(next.x % 2) ? m_gridSize * next.y + m_gridOffset : m_gridSize * next.y + m_gridOffset + m_gridSize * 0.5f);

				float nextLower = go->aLocal[curr.y * m_noGrid + curr.x] + m_myGrid[next.y * m_noGrid + next.x] * (Vector3(nextxPos, nextyPos, 0.f) - Vector3(currxPos, curryPos, 0.f)).Length();

				if (nextLower < go->aLocal[next.y * m_noGrid + next.x])
				{
					go->previous[next.y * m_noGrid + next.x] = curr;
					go->aLocal[next.y * m_noGrid + next.x] = nextLower;
					go->aGlobal[next.y * m_noGrid + next.x] = go->aLocal[next.y * m_noGrid + next.x] + (Vector3(endxPos, endyPos, 0.f) - Vector3(nextxPos, nextyPos, 0.f)).Length();
				}
			}
		}
		if (curr.x < m_noGrid - 1)
		{
			MazePt next(curr.x + 1, curr.y);
			if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] > -1)
			{
				unvisited.push_back(next);

				float nextxPos = m_gridSize * appliedXScale * next.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
				float nextyPos = (!(next.x % 2) ? m_gridSize * next.y + m_gridOffset : m_gridSize * next.y + m_gridOffset + m_gridSize * 0.5f);

				float nextLower = go->aLocal[curr.y * m_noGrid + curr.x] + m_myGrid[next.y * m_noGrid + next.x] * (Vector3(nextxPos, nextyPos, 0.f) - Vector3(currxPos, curryPos, 0.f)).Length();

				if (nextLower < go->aLocal[next.y * m_noGrid + next.x])
				{
					go->previous[next.y * m_noGrid + next.x] = curr;
					go->aLocal[next.y * m_noGrid + next.x] = nextLower;
					go->aGlobal[next.y * m_noGrid + next.x] = go->aLocal[next.y * m_noGrid + next.x] + (Vector3(endxPos, endyPos, 0.f) - Vector3(nextxPos, nextyPos, 0.f)).Length();
				}
			}
		}
		if (curr.x % 2) //if odd x 
		{
			//TOP LEFT 
			if (curr.x - 1 > -1 && curr.y + 1 < m_noGrid)
			{
				MazePt next(curr.x - 1, curr.y + 1);
				if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] > -1)
				{
					unvisited.push_back(next);

					float nextxPos = m_gridSize * appliedXScale * next.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
					float nextyPos = (!(next.x % 2) ? m_gridSize * next.y + m_gridOffset : m_gridSize * next.y + m_gridOffset + m_gridSize * 0.5f);

					float nextLower = go->aLocal[curr.y * m_noGrid + curr.x] + m_myGrid[next.y * m_noGrid + next.x] * (Vector3(nextxPos, nextyPos, 0.f) - Vector3(currxPos, curryPos, 0.f)).Length();

					if (nextLower < go->aLocal[next.y * m_noGrid + next.x])
					{
						go->previous[next.y * m_noGrid + next.x] = curr;
						go->aLocal[next.y * m_noGrid + next.x] = nextLower;
						go->aGlobal[next.y * m_noGrid + next.x] = go->aLocal[next.y * m_noGrid + next.x] + (Vector3(endxPos, endyPos, 0.f) - Vector3(nextxPos, nextyPos, 0.f)).Length();
					}
				}
			}

			//TOP RIGHT 
			if (curr.x + 1 < m_noGrid && curr.y + 1 < m_noGrid)
			{
				MazePt next(curr.x + 1, curr.y + 1);
				if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] > -1)
				{
					unvisited.push_back(next);

					float nextxPos = m_gridSize * appliedXScale * next.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
					float nextyPos = (!(next.x % 2) ? m_gridSize * next.y + m_gridOffset : m_gridSize * next.y + m_gridOffset + m_gridSize * 0.5f);

					float nextLower = go->aLocal[curr.y * m_noGrid + curr.x] + m_myGrid[next.y * m_noGrid + next.x] * (Vector3(nextxPos, nextyPos, 0.f) - Vector3(currxPos, curryPos, 0.f)).Length();

					if (nextLower < go->aLocal[next.y * m_noGrid + next.x])
					{
						go->previous[next.y * m_noGrid + next.x] = curr;
						go->aLocal[next.y * m_noGrid + next.x] = nextLower;
						go->aGlobal[next.y * m_noGrid + next.x] = go->aLocal[next.y * m_noGrid + next.x] + (Vector3(endxPos, endyPos, 0.f) - Vector3(nextxPos, nextyPos, 0.f)).Length();
					}
				}
			}
		}
		else //even x
		{
			//BOT LEFT 
			if (curr.x - 1 > -1 && curr.y - 1 > -1)
			{
				MazePt next(curr.x - 1, curr.y - 1);
				if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] > -1)
				{
					unvisited.push_back(next);

					float nextxPos = m_gridSize * appliedXScale * next.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
					float nextyPos = (!(next.x % 2) ? m_gridSize * next.y + m_gridOffset : m_gridSize * next.y + m_gridOffset + m_gridSize * 0.5f);

					float nextLower = go->aLocal[curr.y * m_noGrid + curr.x] + m_myGrid[next.y * m_noGrid + next.x] * (Vector3(nextxPos, nextyPos, 0.f) - Vector3(currxPos, curryPos, 0.f)).Length();

					if (nextLower < go->aLocal[next.y * m_noGrid + next.x])
					{
						go->previous[next.y * m_noGrid + next.x] = curr;
						go->aLocal[next.y * m_noGrid + next.x] = nextLower;
						go->aGlobal[next.y * m_noGrid + next.x] = go->aLocal[next.y * m_noGrid + next.x] + (Vector3(endxPos, endyPos, 0.f) - Vector3(nextxPos, nextyPos, 0.f)).Length();
					}
				}
			}

			//BOT RIGHT 
			if (curr.x + 1 < m_noGrid && curr.y - 1 > -1)
			{
				MazePt next(curr.x + 1, curr.y - 1);
				if (!go->visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] > -1)
				{
					unvisited.push_back(next);

					float nextxPos = m_gridSize * appliedXScale * next.x * 0.75f + m_gridSize * appliedXScale * 0.5f;
					float nextyPos = (!(next.x % 2) ? m_gridSize * next.y + m_gridOffset : m_gridSize * next.y + m_gridOffset + m_gridSize * 0.5f);

					float nextLower = go->aLocal[curr.y * m_noGrid + curr.x] + m_myGrid[next.y * m_noGrid + next.x] * (Vector3(nextxPos, nextyPos, 0.f) - Vector3(currxPos, curryPos, 0.f)).Length();

					if (nextLower < go->aLocal[next.y * m_noGrid + next.x])
					{
						go->previous[next.y * m_noGrid + next.x] = curr;
						go->aLocal[next.y * m_noGrid + next.x] = nextLower;
						go->aGlobal[next.y * m_noGrid + next.x] = go->aLocal[next.y * m_noGrid + next.x] + (Vector3(endxPos, endyPos, 0.f) - Vector3(nextxPos, nextyPos, 0.f)).Length();
					}
				}
			}
		}
	}

	return false;
}

void SceneTurn::SetUnitStats(GameObject* go)
{
	switch (go->type)
	{
	case GameObject::GO_K9:
		go->visRadius = 3;
		go->health = 100.f;
		go->damage = 20.f;
		go->inventorySize = 5;
		break;
	case GameObject::GO_MINE:
		go->visRadius = 1;
		go->health = 1.f;
		break;
	}
}

void SceneTurn::GenerateEventBombs()
{
	std::vector<int> EmptyTileIndex;
	for (int x = 0; x < m_maze.m_grid.size(); ++x)
	{
		if (m_maze.m_grid[x] == Maze::TILE_EMPTY)
			EmptyTileIndex.push_back(x);
	}

	for (int x = 0; x < 5; ++x)
	{
		int random = Math::RandIntMinMax(0, EmptyTileIndex.size() - 1);
		m_maze.m_grid[EmptyTileIndex[random]] = m_myGrid[EmptyTileIndex[random]] = Maze::TILE_MINE;
		GameObject* mine = new GameObject(GameObject::GO_MINE);
		mine->curr.x = EmptyTileIndex[random] % m_noGrid;
		mine->curr.y = EmptyTileIndex[random] / m_noGrid;
		mine->active = true;
		SetUnitStats(mine);
		mineList.push_back(mine);
		EmptyTileIndex.erase(EmptyTileIndex.begin() + random);
	}
}

void SceneTurn::Update(double dt)
{
	
	SceneBase::Update(dt);
	elapsedTime += dt;

	if (elapsedTime >= eventTime && !eventActive)
	{
		eventActive = true;
		GenerateEventBombs();
	}

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}
	if (Application::IsKeyPressed('N'))
	{
	}
	if (Application::IsKeyPressed('M'))
	{
	}
	if (Application::IsKeyPressed(VK_RETURN))
	{
	}
	if (Application::IsKeyPressed('R'))
	{
		//Exercise: Implement Reset button
	}

	//Input Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		int xIndex;
		int yIndex;
		float dist = FLT_MAX;
		for (int x = 0; x < m_noGrid; ++x)
		{
			float xCentre = m_rightOffset +  m_gridSize * appliedXScale * x * 0.75f + m_gridSize * appliedXScale * 0.5f;
			float offSet = fabs(xCentre - posX);
			if (offSet < dist)
			{
				xIndex = x;
				dist = offSet;
			}
		}
		dist = FLT_MAX;
		for (int y = 0; y < m_noGrid; ++y)
		{
			float yCentre;
			if (xIndex % 2) //odd 
				yCentre = m_gridSize * y + m_gridOffset + m_gridSize * 0.5f;
			else
				yCentre = m_gridSize * y + m_gridOffset;
			float offSet = fabs(yCentre - posY);
			if (offSet < dist)
			{
				yIndex = y;
				dist = offSet;
			}
		}
		m_end.Set(xIndex, yIndex);
		if (target)
			AStar(target, m_end);
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		int xIndex;
		int yIndex;
		float dist = FLT_MAX;
		for (int x = 0; x < m_noGrid; ++x)
		{
			float xCentre = m_rightOffset + m_gridSize * appliedXScale * x * 0.75f + m_gridSize * appliedXScale * 0.5f;
			float offSet = fabs(xCentre - posX);
			if (offSet < dist)
			{
				xIndex = x;
				dist = offSet;
			}
		}
		dist = FLT_MAX;
		for (int y = 0; y < m_noGrid; ++y)
		{
			float yCentre;
			if (xIndex % 2) //odd 
				yCentre = m_gridSize * y + m_gridOffset + m_gridSize * 0.5f;
			else
				yCentre = m_gridSize * y + m_gridOffset;
			float offSet = fabs(yCentre - posY);
			if (offSet < dist)
			{
				yIndex = y;
				dist = offSet;
			}
		}

		m_myGrid[yIndex * m_noGrid + xIndex] = m_maze.m_grid[yIndex * m_noGrid + xIndex] = Maze::TILE_MUD;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	static bool bFState = false;
	if (!bFState && Application::IsKeyPressed('F'))
	{
		bFState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		int xIndex;
		int yIndex;
		float dist = FLT_MAX;
		for (int x = 0; x < m_noGrid; ++x)
		{
			float xCentre = m_rightOffset + m_gridSize * appliedXScale * x * 0.75f + m_gridSize * appliedXScale * 0.5f;
			float offSet = fabs(xCentre - posX);
			if (offSet < dist)
			{
				xIndex = x;
				dist = offSet;
			}
		}
		dist = FLT_MAX;
		for (int y = 0; y < m_noGrid; ++y)
		{
			float yCentre;
			if (xIndex % 2) //odd 
				yCentre = m_gridSize * y + m_gridOffset + m_gridSize * 0.5f;
			else
				yCentre = m_gridSize * y + m_gridOffset;
			float offSet = fabs(yCentre - posY);
			if (offSet < dist)
			{
				yIndex = y;
				dist = offSet;
			}
		}

		m_myGrid[yIndex * m_noGrid + xIndex] = m_maze.m_grid[yIndex * m_noGrid + xIndex] = Maze::TILE_WATER;
	}
	else if (bFState && !Application::IsKeyPressed('F'))
	{
		bFState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
		GameObject *go = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_K9);
		SetUnitStats(go);
		go->grid.resize(m_noGrid * m_noGrid);
		go->visited.resize(m_noGrid * m_noGrid);
		go->aGlobal.resize(m_noGrid * m_noGrid);
		go->aLocal.resize(m_noGrid* m_noGrid);
		std::fill(go->grid.begin(), go->grid.end(), Maze::TILE_FOG);
		std::fill(go->visited.begin(), go->visited.end(), false);
		//set go->curr to an empty tile
		for (int x = 0; x < m_myGrid.size(); ++x)
		{
			if (m_myGrid[x] == Maze::TILE_EMPTY)
			{
				go->curr.Set(x % m_noGrid, x / m_noGrid);
				break;
			}
		}
		go->stack.push_back(go->curr);
		m_myGrid[go->curr.y * m_noGrid + go->curr.x] = Maze::TILE_PLAYER;

		botsideList.push_back(go);
		target = go;
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{   
		bSpaceState = false;
	}

	static bool bRightState = false;
	if (!bRightState && Application::IsKeyPressed(VK_RIGHT))
	{
		bSpaceState = true;
		std::vector<GameObject*> ListCopy = (botsideTurn ? botsideList : topsideList);
		for (int x = 0; x < ListCopy.size(); ++x)
		{
			if (ListCopy[x] == target)
			{
				if (x + 1 < ListCopy.size())
					target = ListCopy[x + 1];
				break;
			}
		}
	}
	else if (bRightState && !Application::IsKeyPressed(VK_RIGHT))
	{
		bRightState = false;
	}

	static bool bLeftState = false;
	if (!bLeftState && Application::IsKeyPressed(VK_LEFT))
	{
		bLeftState = true;
		std::vector<GameObject*> ListCopy = (botsideTurn ? botsideList : topsideList);
		for (int x = 0; x < ListCopy.size(); ++x)
		{
			if (ListCopy[x] == target)
			{
				if (x - 1 > -1)
					target = ListCopy[x - 1];
				break;
			}
		}
	}
	else if (bLeftState && !Application::IsKeyPressed(VK_LEFT))
	{
		bLeftState = false;
	}

	timer += m_speed * dt;
	static const float TURN_TIME = 0.25f;
	if (timer > TURN_TIME)
	{
		m_turn++;
		timer = 0.0;
		std::fill(m_visible.begin(), m_visible.end(), false);
		for (auto go : m_goList)
		{
			if (!go->active)
				return;
			if (!go->path.empty())
			{
				for (int x = 0; x < go->path.size(); ++x)
				{
					if (x == go->path.size() - 1)
					{
						go->path.clear();
					}
					else
					{
						if (go->path[x].x == go->curr.x && go->path[x].y == go->curr.y)
						{
							m_myGrid[go->curr.y * m_noGrid + go->curr.x] = m_maze.m_grid[go->curr.y * m_noGrid + go->curr.x];
							go->curr = go->path[x + 1];
							m_myGrid[go->curr.y * m_noGrid + go->curr.x] = Maze::TILE_PLAYER;
							break;
						}
					}
				}
			}
		}
	}

	//Check of GO has picked up Loot
	for (auto go : m_goList)
	{
		int goIndex = go->curr.y * m_noGrid + go->curr.x;
		for (int x = 0; x < m_maze.m_loot.size(); ++x)
		{
			if (goIndex == m_maze.m_loot[x]->index)
			{
				if (go->inventoryList.size() < go->inventorySize)
				{
					go->inventoryList.push_back(m_maze.m_loot[x]->type);
					m_maze.m_loot.erase(m_maze.m_loot.begin() + x);
				}
			}
		}
	}
}

void SceneTurn::RenderGO(GameObject *go)
{
	modelStack.PushMatrix();
	modelStack.Translate(m_rightOffset + m_gridSize * appliedXScale * go->curr.x * 0.75f + m_gridSize * appliedXScale * 0.5f, m_gridSize * go->curr.y + m_gridOffset + ((go->curr.x % 2) ? m_gridSize * 0.5f : 0), 0);
	modelStack.Scale(m_gridSize * appliedXScale, m_gridSize, m_gridSize);
	switch (go->type)
	{
	case GameObject::GO_K9: //Render GO_NPC
		RenderMesh(meshList[GEO_K9], false);
		break;
	case GameObject::GO_MINE:
		RenderMesh(meshList[GEO_MINE], false);
		break;
	}
	modelStack.PopMatrix();
}

void SceneTurn::RenderLoot(int index, Maze::LOOT_TYPE type)
{
	modelStack.PushMatrix();
	modelStack.Translate(m_rightOffset + m_gridSize * appliedXScale * (index % m_noGrid) * 0.75f + m_gridSize * appliedXScale * 0.5f, m_gridSize * (index / m_noGrid) + m_gridOffset + (((index % m_noGrid) % 2) ? m_gridSize * 0.5f : 0), 0);
	modelStack.Scale(m_gridSize * appliedXScale, m_gridSize, m_gridSize);
	switch (type)
	{
	case Maze::LOOT_FOOD:
		RenderMesh(meshList[GEO_FOOD], false);
		break;
	case Maze::LOOT_DRUG:
		RenderMesh(meshList[GEO_DRUG], false);
		break;
	case Maze::LOOT_HPPACK:
		RenderMesh(meshList[GEO_HPPACK], false);
		break;
	case Maze::LOOT_DMGBOOST:
		RenderMesh(meshList[GEO_DMGBOOST], false);
		break;
	}
	modelStack.PopMatrix();
}

void SceneTurn::UpdateVisibleTiles(GameObject* go, MazePt point, int visRadius)
{
	if (!m_visible[go->curr.y * m_noGrid + go->curr.x])
		m_visible[go->curr.y * m_noGrid + go->curr.x] = true;

	std::vector<MazePt> nextList;
	const static int offset[][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1} };
	for (int x = 0; x < 6; ++x)
	{
		MazePt next;
		if (x < 4)
		{
			next.x = point.x + offset[x][0];
			next.y = point.y + offset[x][1];
		}
		else
		{
			next.x = point.x + offset[x + ((point.x % 2) ? 0 : 2)][0];
			next.y = point.y + offset[x + ((point.x % 2) ? 0 : 2)][1];
		}
		if (next.x < 0 || next.x >= m_noGrid || next.y < 0 || next.y >= m_noGrid)
			continue;
		if (!m_visible[next.y * m_noGrid + next.x])
		{
			go->visIndexes.push_back(next.y * m_noGrid + next.x);
			m_visible[next.y * m_noGrid + next.x] = true;
		}
		if (visRadius > 1)
			nextList.push_back(next);
	}
	for (auto nextL : nextList)
	{
		UpdateVisibleTiles(go, nextL, visRadius - 1);
	}
}

void SceneTurn::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	//RenderMesh(meshList[GEO_AXES], false);

	if (target)
	{
		target->visIndexes.clear();
		UpdateVisibleTiles(target, target->curr, target->visRadius);
		if (eventActive)
		{
			for (auto mine : mineList)
				UpdateVisibleTiles(mine, mine->curr, mine->visRadius);
		}
	}

	//Rendering of map
	for (int y = 0; y < m_noGrid; ++y)
	{
		for (int x = 0; x < m_noGrid; ++x)
		{
			if (!m_visible[y * m_noGrid + x])
				continue;
			modelStack.PushMatrix();
			modelStack.Translate(m_rightOffset + m_gridSize * appliedXScale * x * 0.75f + m_gridSize * appliedXScale * 0.5f, m_gridSize * y + m_gridOffset + ((x % 2) ? m_gridSize * 0.5f : 0), 0);
			modelStack.Scale(m_gridSize * appliedXScale, m_gridSize, m_gridSize);
			switch (m_myGrid[y * m_noGrid + x])
			{
			case Maze::TILE_WALL:
				RenderMesh(meshList[GEO_HEXBLACK], false);
				break;
			case Maze::TILE_FOG:
				RenderMesh(meshList[GEO_HEXGREY], false);
				break;
			case Maze::TILE_EMPTY:
				RenderMesh(meshList[GEO_HEXWHITE], false);
				break;
			case Maze::TILE_WATER:
				RenderMesh(meshList[GEO_WATER], false);
				break;
			case Maze::TILE_MUD:
				RenderMesh(meshList[GEO_MUD], false);
				break;
			}
			modelStack.PopMatrix();
		}
	}

	//Rendering of Loot
	for (auto loot : m_maze.m_loot)
	{
		if (!m_visible[loot->index])
			continue;
		RenderLoot(loot->index, loot->type);
	}

	//Rendering of GOs
	for (auto go : m_goList)
	{
		if (!m_visible[go->curr.y * m_noGrid + go->curr.x])
			continue;
		if (go->active)
			RenderGO(go);
	}

	//Rendering Mines
	for (auto mine : mineList)
	{
		if (!m_visible[mine->curr.y * m_noGrid + mine->curr.x])
			continue;
		if (mine->active)
			RenderGO(mine);
	}

	//Render Target Border
	if (target)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_rightOffset + m_gridSize * appliedXScale * target->curr.x * 0.75f + m_gridSize * appliedXScale * 0.5f, m_gridSize * target->curr.y + m_gridOffset + ((target->curr.x % 2) ? m_gridSize * 0.5f : 0), 0);
		modelStack.Scale(m_gridSize * appliedXScale * 1.2f, m_gridSize * 1.2f, m_gridSize * 1.2f);
		RenderMesh(meshList[GEO_BORDER], false);
		modelStack.PopMatrix();
	}

	//On screen text
	std::ostringstream ss;

	ss.precision(3);
	ss.str("");
	ss << "Elapsed Time: " << elapsedTime;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.f, 0.f, 0.f), 3, 58, 12);

	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.f, 0.f, 0.f), 3, 58, 6);
																			  
	ss.str("");																  
	ss.precision(5);														  
	ss << "FPS:" << fps;													  
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.f, 0.f, 0.f), 3, 58, 3);
																			  
	ss.str("");																  
	ss << "Turn:" << m_turn;												  
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.f, 0.f, 0.f), 3, 58, 9);
														   					 
	ss.str("");											   					 
	ss << "Turn Maze " << m_mazeKey;					   					 
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.f, 0.f, 0.f), 3, 58, 0);

	if (target)
	{
		std::string name;
		switch (target->type)
		{
		case GameObject::GO_K9:
			name = "K9";
		}

		ss.str("");
		ss << "Unit: " << name;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.f, 1.f, 0.f), 3, 58, 54);

		ss.str("");
		ss << "Health: " << target->health;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.f, 1.f, 0.f), 3, 58, 51);

		ss.str("");
		ss << "Damage: " << target->damage;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.f, 1.f, 0.f), 3, 58, 48);

		ss.str("");
		ss << "Inventory: " << target->inventoryList.size() << "/" << target->inventorySize;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.f, 1.f, 0.f), 3, 58, 45);

		for (int x = 0; x < target->inventoryList.size(); ++x)
		{
			modelStack.PushMatrix();
			modelStack.Translate(142.f + (5.f * x), 70.f, 0.f); 
			modelStack.Scale(m_gridSize* appliedXScale * 1.5f, m_gridSize * 1.5f, m_gridSize * 1.5f);
			switch (target->inventoryList[x])
			{
			case Maze::LOOT_DMGBOOST:
				RenderMesh(meshList[GEO_DMGBOOST], false);
				break;
			case Maze::LOOT_DRUG:
				RenderMesh(meshList[GEO_DRUG], false);
				break;
			case Maze::LOOT_FOOD:
				RenderMesh(meshList[GEO_FOOD], false);
				break;
			case Maze::LOOT_HPPACK:
				RenderMesh(meshList[GEO_HPPACK], false);
				break;
			}
			modelStack.PopMatrix();
		}
	}
}

void SceneTurn::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}
