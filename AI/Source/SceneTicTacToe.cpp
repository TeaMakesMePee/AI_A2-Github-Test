#include "SceneTicTacToe.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneTicTacToe::SceneTicTacToe()
{
}

SceneTicTacToe::~SceneTicTacToe()
{
}

void SceneTicTacToe::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;
	m_noGrid = 3;
	m_gridSize = m_worldHeight / m_noGrid;
	m_gridOffset = m_gridSize / 2;

	int rand = Math::RandIntMinMax(1, 2);
	if (rand == 1)
		m_bCrossTurn = true;
	else
		m_bCrossTurn = false;
	m_bGameOver = false;
	m_winner = WHO_CONTENT::WHO_NONE;
	m_bSecondAI = false;
}

GameObject* SceneTicTacToe::FetchGO()
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

//Exercise: This function should return 0 to 8, i.e. AI player's decision to put circle on one of the grids

void SceneTicTacToe::Update(double dt)
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
		m_bGameOver = false;
		m_bSecondAI = false;
		int rand = Math::RandIntMinMax(1, 2);
		if (rand == 1)
			m_bCrossTurn = true;
		else
			m_bCrossTurn = false;
		m_objectCount = 0;
		for (int x = 0; x < 9; ++x)
		{
			m_grid[x] = WHO_NONE;
		}
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				go->active = false;
			}
		}
	}
	if (Application::IsKeyPressed('A')) //for ai vs ai
	{
		m_bGameOver = false;
		int rand = Math::RandIntMinMax(1, 2);
		if (rand == 1)
			m_bCrossTurn = true;
		else
			m_bCrossTurn = false;
		m_objectCount = 0;
		for (int x = 0; x < 9; ++x)
		{
			m_grid[x] = WHO_NONE;
		}
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				go->active = false;
			}
		}
		m_bSecondAI = true;
	}

	//Input Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0) && !m_bSecondAI)
	{
		bLButtonState = true;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		//Exercise: Game input
		int gridX = posX / m_gridSize;
		int gridY = posY / m_gridSize; 
		int gridNum = gridX + gridY * 3;
		if (m_bCrossTurn && !m_bGameOver)
		{
			GameObject* input = FetchGO();
			input->type = GameObject::GO_CROSS;
			input->pos = Vector3(m_gridSize * gridX + m_gridOffset, m_gridSize * gridY + m_gridOffset);
			input->active = true;
			input->scale.Set(m_gridSize, m_gridSize, 0.f);
			m_grid[gridNum] = WHO_CROSS;
			m_bCrossTurn = false;
			m_aibouncetime = 1.0;
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
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

	if (!m_bCrossTurn && !m_bGameOver)
	{
		//Exercise: Call GetAIDecision() and put circle on grid
		if (m_aibouncetime <= 0.0)
		{
			GetAIDecision();
			if (m_bSecondAI)
			{
				m_aibouncetime = 1.0;
				m_bCrossTurn = true;
			}
		}
		m_aibouncetime -= dt;
	}
	else if (m_bCrossTurn && m_bSecondAI && !m_bGameOver)
	{
		if (m_aibouncetime <= 0.0)
		{
			GetAIDecision();
			m_aibouncetime = 1.0;
			m_bCrossTurn = false;
		}
		m_aibouncetime -= dt;
	}

	//Game Logic Section
	//Exercise: Check draw, cross wins or circle wins

	//vertical check
	for (int x = 0; x < 3; ++x) 
	{
		WHO_CONTENT checktype = m_grid[x];
		if (checktype == WHO_NONE)
			continue;
		bool first, second;
		first = second = false;
		for (int y = x + 3; y < x + 7; y += 3)
		{
			if (first)
				if (m_grid[y] == checktype)
					second = true;
			if (y == x + 3)
				if (m_grid[y] == checktype)
					first = true;
			if (first && second)
			{
				m_winner = checktype; 
				m_bGameOver = true;
			}
		}
	}

	//horizontal check 
	for (int x = 0; x < 7; x += 3)
	{
		WHO_CONTENT checktype = m_grid[x];
		if (checktype == WHO_NONE)
			continue;
		bool first, second;
		first = second = false;
		for (int y = x + 1; y < x + 3; ++y)
		{
			if (first)
				if (m_grid[y] == checktype)
					second = true;
			if (y == x + 1)
				if (m_grid[y] == checktype)
					first = true;
			if (first && second)
			{
				m_winner = checktype;
				m_bGameOver = true;
			}
		}
	}

	//diagonal check
	if (m_grid[4] != WHO_NONE)
	{
		if (m_grid[2] == m_grid[4] && m_grid[4] == m_grid[6])
		{
			m_winner = m_grid[2];
			m_bGameOver = true;
		}
		if (m_grid[0] == m_grid[4] && m_grid[4] == m_grid[8])
		{
			m_winner = m_grid[0];
			m_bGameOver = true;
		}
	}

	if (m_objectCount == 9 && !m_bGameOver)
	{
		m_winner = WHO_NONE;
		m_bGameOver = true;
	}
}	

void SceneTicTacToe::GetAIDecision()
{
	//check if can win first
	for (int x = 0; x < 3; ++x)
	{
		int count = 0;
		int whichgrid = 10;
		for (int y = x; y < x + 7; y += 3)
		{
			if (!m_bCrossTurn)
			{
				if (m_grid[y] == WHO_CIRCLE)
					count++;
				else if (m_grid[y] == WHO_NONE)
					whichgrid = y;
			}
			else
			{
				if (m_grid[y] == WHO_CROSS)
					count++;
				else if (m_grid[y] == WHO_NONE)
					whichgrid = y;
			}
		}
		if (count == 2 && whichgrid != 10)
		{
			GameObject* ai = FetchGO();
			int y = whichgrid / 3;
			int x = whichgrid - y * 3;
			ai->pos = Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
			if (!m_bCrossTurn)
			{
				ai->type = GameObject::GO_CIRCLE;
				m_grid[whichgrid] = WHO_CIRCLE;
			}
			else
			{
				ai->type = GameObject::GO_CROSS;
				m_grid[whichgrid] = WHO_CROSS;
			}
			ai->active = true;
			ai->scale.Set(m_gridSize, m_gridSize, 0.f);
			m_bCrossTurn = true;
			return;
		}
	}

	for (int x = 0; x < 7; x += 3)
	{
		int count = 0;
		int whichgrid = 10;
		for (int y = x; y < x + 3; ++y)
		{
			if (!m_bCrossTurn)
			{
				if (m_grid[y] == WHO_CIRCLE)
					count++;
				else if (m_grid[y] == WHO_NONE)
					whichgrid = y;
			}
			else
			{
				if (m_grid[y] == WHO_CROSS)
					count++;
				else if (m_grid[y] == WHO_NONE)
					whichgrid = y;
			}
		}
		if (count == 2 && whichgrid != 10)
		{
			GameObject* ai = FetchGO();
			int y = whichgrid / 3;
			int x = whichgrid - y * 3;
			ai->pos = Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
			if (!m_bCrossTurn)
			{
				ai->type = GameObject::GO_CIRCLE;
				m_grid[whichgrid] = WHO_CIRCLE;
			}
			else
			{
				ai->type = GameObject::GO_CROSS;
				m_grid[whichgrid] = WHO_CROSS;
			}
			ai->active = true;
			ai->scale.Set(m_gridSize, m_gridSize, 0.f);
			m_bCrossTurn = true;
			return;
		}
	}

	int whichgrid = 10;
	int count = 0;

	for (int x = 2; x < 7; x += 2)
	{
		if (!m_bCrossTurn)
		{
			if (m_grid[x] == WHO_CIRCLE)
				count++;
			else
				if (m_grid[x] == WHO_NONE)
					whichgrid = x;
		}
		else
		{
			if (m_grid[x] == WHO_CROSS)
				count++;
			else
				if (m_grid[x] == WHO_NONE)
					whichgrid = x;
		}
	}

	if (count == 2 && whichgrid != 10)
	{
		GameObject* ai = FetchGO();
		int y = whichgrid / 3;
		int x = whichgrid - y * 3;
		ai->pos = Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
		if (!m_bCrossTurn)
		{
			ai->type = GameObject::GO_CIRCLE;
			m_grid[whichgrid] = WHO_CIRCLE;
		}
		else
		{
			ai->type = GameObject::GO_CROSS;
			m_grid[whichgrid] = WHO_CROSS;
		}
		ai->active = true;
		ai->scale.Set(m_gridSize, m_gridSize, 0.f);
		m_bCrossTurn = true;
		return;
	}

	count = 0;
	whichgrid = 10;

	for (int x = 0; x < 9; x += 4)
	{
		if (!m_bCrossTurn)
		{
			if (m_grid[x] == WHO_CIRCLE)
				count++;
			else
				if (m_grid[x] == WHO_NONE)
					whichgrid = x;
		}
		else
		{
			if (m_grid[x] == WHO_CROSS)
				count++;
			else
				if (m_grid[x] == WHO_NONE)
					whichgrid = x;
		}
	}

	if (count == 2 && whichgrid != 10)
	{
		GameObject* ai = FetchGO();
		int y = whichgrid / 3;
		int x = whichgrid - y * 3;
		ai->pos = Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
		if (!m_bCrossTurn)
		{
			ai->type = GameObject::GO_CIRCLE;
			m_grid[whichgrid] = WHO_CIRCLE;
		}
		else
		{
			ai->type = GameObject::GO_CROSS;
			m_grid[whichgrid] = WHO_CROSS;
		}
		ai->active = true;
		ai->scale.Set(m_gridSize, m_gridSize, 0.f);
		m_bCrossTurn = true;
		return;
	}
	
	///////check to defend/////////
	for (int x = 0; x < 3; ++x)
	{
		int count = 0;
		int whichgrid = 10;
		for (int y = x; y < x + 7; y += 3)
		{
			if (!m_bCrossTurn)
			{
				if (m_grid[y] == WHO_CROSS)
					count++;
				else if (m_grid[y] == WHO_NONE)
					whichgrid = y;
			}
			else
			{
				if (m_grid[y] == WHO_CIRCLE)
					count++;
				else if (m_grid[y] == WHO_NONE)
					whichgrid = y;
			}
		}
		if (count == 2 && whichgrid != 10)
		{
			GameObject* ai = FetchGO();
			int y = whichgrid / 3;
			int x = whichgrid - y * 3;
			ai->pos = Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
			if (!m_bCrossTurn)
			{
				ai->type = GameObject::GO_CIRCLE;
				m_grid[whichgrid] = WHO_CIRCLE;
			}
			else
			{
				ai->type = GameObject::GO_CROSS;
				m_grid[whichgrid] = WHO_CROSS;
			}
			ai->active = true;
			ai->scale.Set(m_gridSize, m_gridSize, 0.f);
			m_bCrossTurn = true;
			return;
		}
	}

	for (int x = 0; x < 7; x += 3)
	{
		int count = 0;
		int whichgrid = 10;
		for (int y = x; y < x + 3; ++y)
		{
			if (!m_bCrossTurn)
			{
				if (m_grid[y] == WHO_CROSS)
					count++;
				else if (m_grid[y] == WHO_NONE)
					whichgrid = y;
			}
			else
			{
				if (m_grid[y] == WHO_CIRCLE)
					count++;
				else if (m_grid[y] == WHO_NONE)
					whichgrid = y;
			}
		}
		if (count == 2 && whichgrid != 10)
		{
			GameObject* ai = FetchGO();
			int y = whichgrid / 3;
			int x = whichgrid - y * 3;
			ai->pos = Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
			if (!m_bCrossTurn)
			{
				ai->type = GameObject::GO_CIRCLE;
				m_grid[whichgrid] = WHO_CIRCLE;
			}
			else
			{
				ai->type = GameObject::GO_CROSS;
				m_grid[whichgrid] = WHO_CROSS;
			}
			ai->active = true;
			ai->scale.Set(m_gridSize, m_gridSize, 0.f);
			m_bCrossTurn = true;
			return;
		}
	}

	whichgrid = 10;
	count = 0;
	
	for (int x = 2; x < 7; x += 2)
	{
		if (!m_bCrossTurn)
		{
			if (m_grid[x] == WHO_CROSS)
				count++;
			else if (m_grid[x] == WHO_NONE)
				whichgrid = x;
		}
		else
		{
			if (m_grid[x] == WHO_CIRCLE)
				count++;
			else if (m_grid[x] == WHO_NONE)
				whichgrid = x;
		}
	}

	if (count == 2 && whichgrid != 10)
	{
		GameObject* ai = FetchGO();
		int y = whichgrid / 3;
		int x = whichgrid - y * 3;
		ai->pos = Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
		if (!m_bCrossTurn)
		{
			ai->type = GameObject::GO_CIRCLE;
			m_grid[whichgrid] = WHO_CIRCLE;
		}
		else
		{
			ai->type = GameObject::GO_CROSS;
			m_grid[whichgrid] = WHO_CROSS;
		}
		ai->active = true;
		ai->scale.Set(m_gridSize, m_gridSize, 0.f);
		m_bCrossTurn = true;
		return;
	}

	count = 0;
	whichgrid = 10;

	for (int x = 0; x < 9; x += 4)
	{
		if (!m_bCrossTurn)
		{
			if (m_grid[x] == WHO_CROSS)
				count++;
			else if (m_grid[x] == WHO_NONE)
				whichgrid = x;
		}
		else
		{
			if (m_grid[x] == WHO_CIRCLE)
				count++;
			else if (m_grid[x] == WHO_NONE)
				whichgrid = x;
		}
	}

	if (count == 2 && whichgrid != 10)
	{
		GameObject* ai = FetchGO();
		int y = whichgrid / 3;
		int x = whichgrid - y * 3;
		ai->pos = Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
		if (!m_bCrossTurn)
		{
			ai->type = GameObject::GO_CIRCLE;
			m_grid[whichgrid] = WHO_CIRCLE;
		}
		else
		{
			ai->type = GameObject::GO_CROSS;
			m_grid[whichgrid] = WHO_CROSS;
		}
		ai->active = true;
		ai->scale.Set(m_gridSize, m_gridSize, 0.f);
		m_bCrossTurn = true;
		return;
	}

	//if cant attack or defend then place randomly
	int unoccupied = 0;
	int rand;
	std::vector<int> gridPos;
	for (int x = 0; x < 9; ++x)
	{
		if (m_grid[x] == WHO_NONE)
		{
			unoccupied++;
			gridPos.push_back(x);
		}
	}
	if (unoccupied > 0)
	{
		rand = Math::RandIntMinMax(0, unoccupied - 1);
		GameObject* ai = FetchGO();
		int y = gridPos[rand] / 3;
		int x = gridPos[rand] - y * 3;
		ai->pos = Vector3(m_gridSize * x + m_gridOffset, m_gridSize * y + m_gridOffset);
		if (!m_bCrossTurn)
		{
			ai->type = GameObject::GO_CIRCLE;
			m_grid[gridPos[rand]] = WHO_CIRCLE;
		}
		else
		{
			ai->type = GameObject::GO_CROSS;
			m_grid[gridPos[rand]] = WHO_CROSS;
		}
		ai->active = true;
		ai->scale.Set(m_gridSize, m_gridSize, 0.f);
		m_bCrossTurn = true;
	}
}

void SceneTicTacToe::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_CROSS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CROSS], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CIRCLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CIRCLE], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneTicTacToe::Render()
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

	modelStack.PushMatrix();
	modelStack.Translate(m_worldHeight * 0.5f, m_worldHeight * 0.5f, -1.f);
	modelStack.Scale(m_worldHeight, m_worldHeight, m_worldHeight);
	RenderMesh(meshList[GEO_TICTACTOE], false);
	modelStack.PopMatrix();

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text
	static std::string winnerText[] = {"Draw", "Cross wins", "Circle wins"};

	std::ostringstream ss;
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Count:" << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss << "Turn:" << (m_bCrossTurn ? "Cross" : "Circle");
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);

	if (m_bGameOver)
	{
		ss.str("");
		ss << winnerText[static_cast<int>(m_winner)];
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 15);
	}

	RenderTextOnScreen(meshList[GEO_TEXT], "Tic Tac Toe (R to reset)", Color(0, 1, 0), 3, 50, 0);
}

void SceneTicTacToe::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
