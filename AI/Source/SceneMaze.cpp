#include "SceneMaze.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneMaze::SceneMaze()
{
}

SceneMaze::~SceneMaze()
{
}

void SceneMaze::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();
	
	m_noGrid = 12;
	m_gridSize = m_worldHeight / m_noGrid;
	m_gridOffset = m_gridSize / 2;

	m_start.Set(0, 0);
	//m_end.Set(1, 3);
	m_curr.Set(0, 0);
	m_mazeKey = 0;
	m_maze.Generate(m_mazeKey, m_noGrid, m_start, 0.3);
	m_myGrid.resize(m_noGrid * m_noGrid, Maze::TILE_FOG);
	m_visited.resize(m_noGrid * m_noGrid, false);
	m_previous.resize(m_noGrid * m_noGrid);
	m_myGrid[m_start.y * m_noGrid + m_start.x] = Maze::TILE_EMPTY;
	m_maze.SetCurr(m_curr);
	DFS(m_start);
	//BFS(m_start, m_end);
}

GameObject* SceneMaze::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			++m_objectCount;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_CROSS);
		m_goList.push_back(go);
	}
	return FetchGO();
}

void SceneMaze::DFS(MazePt curr)
{
	int index = m_maze.GetCurr().y * m_noGrid + m_maze.GetCurr().x;
	m_visited[index] = true;

	if (index + m_noGrid < m_noGrid * m_noGrid)
	{
		if (!m_visited[index + m_noGrid]) //check up
		{
			if (m_maze.Move(Maze::DIR_UP))
			{
				m_myGrid[index + m_noGrid] = Maze::TILE_EMPTY;
				DFS(m_curr);
				m_maze.Move(Maze::DIR_DOWN);
			}
			else
			{
				m_myGrid[index + m_noGrid] = Maze::TILE_WALL;
			}
		}
	}

	if (index - m_noGrid >= 0)
	{
		if (!m_visited[index - m_noGrid])
		{
			if (m_maze.Move(Maze::DIR_DOWN))
			{
				m_myGrid[index - m_noGrid] = Maze::TILE_EMPTY;
				DFS(m_curr);
				m_maze.Move(Maze::DIR_UP);
			}
			else
			{
				m_myGrid[index - m_noGrid] = Maze::TILE_WALL;

			}
		}
	}

	int newIndex = m_maze.GetCurr().y * m_noGrid + m_maze.GetCurr().x - 1;
	if (m_maze.GetCurr().x - 1 >= 0)
	{
		if (!m_visited[newIndex])
		{
			if (m_maze.Move(Maze::DIR_LEFT))
			{
				m_myGrid[newIndex] = Maze::TILE_EMPTY;
				DFS(curr);
				m_maze.Move(Maze::DIR_RIGHT);
			}
			else
			{
				m_myGrid[newIndex] = Maze::TILE_WALL;
			}
		}
	}


	newIndex = m_maze.GetCurr().y * m_noGrid + m_maze.GetCurr().x + 1;
	if (m_maze.GetCurr().x + 1 < m_noGrid)
	{
		if (!m_visited[newIndex])
		{
			if (m_maze.Move(Maze::DIR_RIGHT))
			{
				m_myGrid[newIndex] = Maze::TILE_EMPTY;
				DFS(curr);
				m_maze.Move(Maze::DIR_LEFT);
			}
			else
			{
				m_myGrid[newIndex] = Maze::TILE_WALL;
			}
		}
	}
}

bool SceneMaze::BFS(MazePt start, MazePt end)
{
	std::fill(m_visited.begin(), m_visited.end(), false);
	std::fill(m_previous.begin(), m_previous.end(), MazePt(0, 0));
	m_shortestPath.clear();
	m_maze.SetNumMove(0);

	while (!m_queue.empty())
	{
		m_queue.pop();
	}

	m_queue.push(start);

	while (!m_queue.empty())
	{
		m_curr = m_queue.front();
		m_queue.pop();
		if (m_curr.x == end.x && m_curr.y == end.y)
		{
			while (!(m_curr.x == start.x && m_curr.y == start.y))
			{
				m_shortestPath.push_back(m_curr);
				m_curr = m_previous[m_curr.y * m_noGrid + m_curr.x];
			}

			m_shortestPath.push_back(m_curr);
			std::reverse(m_shortestPath.begin(), m_shortestPath.end());
			Application::GetInstance().Iterate();
			m_maze.SetCurr(end);
			return true;
		}

		m_maze.SetCurr(m_curr);

		if (m_maze.Move(Maze::DIR_UP))
		{
			if (!m_visited[(m_curr.y + 1) * m_noGrid + m_curr.x])
			{
				m_previous[(m_curr.y + 1) * m_noGrid + m_curr.x] = m_curr;
				m_queue.push(m_maze.GetCurr());
				m_visited[(m_curr.y + 1) * m_noGrid + m_curr.x] = true;
				m_myGrid[(m_curr.y + 1) * m_noGrid + m_curr.x] = Maze::TILE_EMPTY;
			}
		}

		m_maze.SetCurr(m_curr);
		if (m_maze.Move(Maze::DIR_DOWN))
		{
			if (!m_visited[(m_curr.y - 1) * m_noGrid + m_curr.x])
			{
				m_previous[(m_curr.y - 1) * m_noGrid + m_curr.x] = m_curr;
				m_queue.push(m_maze.GetCurr());
				m_visited[(m_curr.y - 1) * m_noGrid + m_curr.x] = true;
				m_myGrid[(m_curr.y - 1) * m_noGrid + m_curr.x] = Maze::TILE_EMPTY;
			}
		}

		m_maze.SetCurr(m_curr);
		if (m_maze.Move(Maze::DIR_LEFT))
		{
			if (!m_visited[m_curr.y * m_noGrid + m_curr.x - 1])
			{
				m_previous[m_curr.y * m_noGrid + m_curr.x - 1] = m_curr;
				m_queue.push(m_maze.GetCurr());
				m_visited[m_curr.y * m_noGrid + m_curr.x - 1] = true;
				m_myGrid[m_curr.y * m_noGrid + m_curr.x - 1] = Maze::TILE_EMPTY;
			}
		}

		m_maze.SetCurr(m_curr);
		if (m_maze.Move(Maze::DIR_RIGHT))
		{
			if (!m_visited[m_curr.y * m_noGrid + m_curr.x + 1])
			{
				m_previous[m_curr.y * m_noGrid + m_curr.x + 1] = m_curr;
				m_queue.push(m_maze.GetCurr());
				m_visited[m_curr.y * m_noGrid + m_curr.x + 1] = true;
				m_myGrid[m_curr.y * m_noGrid + m_curr.x + 1] = Maze::TILE_EMPTY;
			}
		}
	}
	return false;
}

void SceneMaze::Update(double dt)
{
	SceneBase::Update(dt);

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
		
		int yy = posY / m_gridSize;
		int xx = posX / m_gridSize;
		std::cout << xx << " " << yy << std::endl;
		m_end.Set(xx, yy);
		BFS(m_start, m_end);
		//Exercise: turn mouse click into end point and run BFS
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
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}
}


void SceneMaze::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_NONE:
		break;
	}
}

void SceneMaze::Render()
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

	RenderMesh(meshList[GEO_AXES], false);
	for (int x = 0; x < m_shortestPath.size(); ++x)
	{
		int gridX = m_shortestPath[x].x * m_gridSize + m_gridOffset;
		int gridY = m_shortestPath[x].y * m_gridSize + m_gridOffset;

		modelStack.PushMatrix();
		modelStack.Translate(gridX, gridY, -1.f);
		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
		RenderMesh(meshList[GEO_QUEEN], false);
		modelStack.PopMatrix();

	}


	for (int x = 0; x < m_noGrid * m_noGrid; ++x)
	{
		Maze::TILE_CONTENT curContent = m_myGrid[x];
		int gridX = x % m_noGrid;
		int gridY = x / m_noGrid;

		switch (curContent)
		{
		case Maze::TILE_EMPTY:
		{
			modelStack.PushMatrix();
			modelStack.Translate(gridX * m_gridSize + m_gridOffset, gridY * m_gridSize + m_gridOffset, -1.f);
			modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
			RenderMesh(meshList[GEO_WHITEQUAD], false);
			modelStack.PopMatrix();
		}
		break;
		case Maze::TILE_WALL:
		{
			modelStack.PushMatrix();
			modelStack.Translate(gridX * m_gridSize + m_gridOffset, gridY * m_gridSize + m_gridOffset, -1.f);
			modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
			RenderMesh(meshList[GEO_GREYQUAD], false);
			modelStack.PopMatrix();
		}
		break;
		case Maze::TILE_FOG:
		{
			modelStack.PushMatrix();
			modelStack.Translate(gridX * m_gridSize + m_gridOffset, gridY * m_gridSize + m_gridOffset, -1.f);
			modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
			RenderMesh(meshList[GEO_BLACKQUAD], false);
			modelStack.PopMatrix();
		}
		break;
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(m_worldHeight * 0.5f, m_worldHeight * 0.5f, -1.f);
	modelStack.Scale(m_worldHeight, m_worldHeight, m_worldHeight);
	RenderMesh(meshList[GEO_WHITEQUAD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_maze.GetCurr().x * m_gridSize + m_gridOffset, m_maze.GetCurr().y * m_gridSize + m_gridOffset, 0);
	modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
	RenderMesh(meshList[GEO_QUEEN], false);
	modelStack.PopMatrix();

	//Render tiles 
	//Render curr point
	//Render shortest path

	//On screen text
	std::ostringstream ss;
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Num Move:" << 0;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss << "Maze " << 0;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 0);
}

void SceneMaze::Exit()
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
