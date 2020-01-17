#include "SceneReversi.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneReversi::SceneReversi()
{
}

SceneReversi::~SceneReversi()
{
}

void SceneReversi::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_noGrid = 8;
	m_gridSize = m_worldHeight / 8;
	m_gridOffset = m_gridSize / 2;
	m_grid.resize(m_noGrid * m_noGrid, WHO_NONE);
	Reset();
}

void SceneReversi::Reset()
{
	//Exercise: Implement reset
	m_grid.resize(m_noGrid * m_noGrid, WHO_NONE);
	m_speed = 1.f;
	m_bGameOver = false;
	m_bBlackTurn = true;
	int count = 1;
	for (int x = m_noGrid * 0.5f - 1; x < m_noGrid * 0.5f + 1; ++x)
	{
		for (int y = m_noGrid * 0.5f - 1; y < m_noGrid * 0.5f + 1; ++y)
		{	
			if (count == 1 || count == 4)
				m_grid[y * m_noGrid + x] = WHO_BLACK;
			else
				m_grid[y * m_noGrid + x] = WHO_WHITE;
			count++;
		}
	}
}

bool SceneReversi::Move(std::vector<WHO_CONTENT>& grid, bool black, int index)
{
	int xCord = index % m_noGrid;
	int yCord = index / m_noGrid;

	bool moved = false;

	//Up
	if (yCord + 2 < m_noGrid)
	{
		int newY = yCord + 1;
		int newX = xCord;
		if (grid[(yCord + 1) * m_noGrid + xCord] == WHO_WHITE && black)
		{
			while (newY)
			if (grid[(yCord + 2) * m_noGrid + xCord] == WHO_BLACK)
			{
				grid[(yCord + 1) * m_noGrid + xCord] = grid[yCord * m_noGrid + xCord] = WHO_BLACK;
				moved = true;
			}
		}
		else if (grid[(yCord + 1) * m_noGrid + xCord] == WHO_BLACK && !black)
		{
			if (grid[(yCord + 2) * m_noGrid + xCord] == WHO_WHITE)
			{
				grid[(yCord + 1) * m_noGrid + xCord] = grid[yCord * m_noGrid + xCord] = WHO_WHITE;
				moved = true;
			}
		}
	}

	//Down
	if (yCord - 2 > -1)
	{
		if (grid[(yCord - 1) * m_noGrid + xCord] == WHO_WHITE && black)
		{
			if (grid[(yCord - 2) * m_noGrid + xCord] == WHO_BLACK)
			{
				grid[(yCord - 1) * m_noGrid + xCord] = grid[yCord * m_noGrid + xCord] = WHO_BLACK;
				moved = true;
			}
		}
		else if (grid[(yCord - 1) * m_noGrid + xCord] == WHO_BLACK && !black)
		{
			if (grid[(yCord - 2) * m_noGrid + xCord] == WHO_WHITE)
			{
				grid[(yCord - 1) * m_noGrid + xCord] = grid[yCord * m_noGrid + xCord] = WHO_WHITE;
				moved = true;
			}
		}
	}

	//Left
	if (xCord - 2 > -1)
	{
		if (grid[yCord * m_noGrid + xCord - 1] == WHO_WHITE && black)
		{
			if (grid[yCord * m_noGrid + xCord - 2] == WHO_BLACK)
			{
				grid[yCord * m_noGrid + xCord - 1] = grid[yCord * m_noGrid + xCord] = WHO_BLACK;
				moved = true;
			}
		}
		else if (grid[yCord * m_noGrid + xCord - 1] == WHO_BLACK && !black)
		{
			if (grid[yCord * m_noGrid + xCord - 2] == WHO_WHITE)
			{
				grid[yCord * m_noGrid + xCord - 1] = grid[yCord * m_noGrid + xCord] = WHO_WHITE;
				moved = true;
			}
		}
	}

	//Right 
	if (xCord + 2 < m_noGrid)
	{
		if (grid[yCord * m_noGrid + xCord + 1] == WHO_WHITE && black)
		{
			if (grid[yCord * m_noGrid + xCord + 2] == WHO_BLACK)
			{
				grid[yCord * m_noGrid + xCord + 1] = grid[yCord * m_noGrid + xCord] = WHO_BLACK;
				moved = true;
			}
		}
		else if (grid[yCord * m_noGrid + xCord + 1] == WHO_BLACK && !black)
		{
			if (grid[yCord * m_noGrid + xCord + 2] == WHO_WHITE)
			{
				grid[yCord * m_noGrid + xCord + 1] = grid[yCord * m_noGrid + xCord] = WHO_WHITE;
				moved = true;
			}
		}
	}

	//Top-Right
	if (xCord + 2 < m_noGrid && yCord + 2 < m_noGrid)
	{
		if (grid[(yCord + 1) * m_noGrid + xCord + 1] == WHO_WHITE && black)
		{
			if (grid[(yCord + 2) * m_noGrid + xCord + 2] == WHO_BLACK)
			{
				grid[(yCord + 1) * m_noGrid + xCord + 1] = grid[yCord * m_noGrid + xCord] = WHO_BLACK;
				moved = true;
			}
		}
		else if (grid[(yCord + 1) * m_noGrid + xCord + 1] == WHO_BLACK && !black)
		{
			if (grid[(yCord + 2) * m_noGrid + xCord + 2] == WHO_WHITE)
			{
				grid[(yCord + 1) * m_noGrid + xCord + 1] = grid[yCord * m_noGrid + xCord] = WHO_WHITE;
				moved = true;
			}
		}
	}

	//Top-Left
	if (xCord - 2 > -1 && yCord + 2 < m_noGrid)
	{
		if (grid[(yCord + 1) * m_noGrid + xCord - 1] == WHO_WHITE && black)
		{
			if (grid[(yCord + 2) * m_noGrid + xCord - 2] == WHO_BLACK)
			{
				grid[(yCord + 1) * m_noGrid + xCord - 1] = grid[yCord * m_noGrid + xCord] = WHO_BLACK;
				moved = true;
			}
		}
		else if (grid[(yCord + 1) * m_noGrid + xCord - 1] == WHO_BLACK && !black)
		{
			if (grid[(yCord + 2) * m_noGrid + xCord - 2] == WHO_WHITE)
			{
				grid[(yCord + 1) * m_noGrid + xCord - 1] = grid[yCord * m_noGrid + xCord] = WHO_WHITE;
				moved = true;
			}
		}
	}

	//Bottom Left
	if (xCord - 2 > -1 && yCord - 2 > -1)
	{
		if (grid[(yCord - 1) * m_noGrid + xCord - 1] == WHO_WHITE && black)
		{
			if (grid[(yCord - 2) * m_noGrid + xCord - 2] == WHO_BLACK)
			{
				grid[(yCord - 1) * m_noGrid + xCord - 1] = grid[yCord * m_noGrid + xCord] = WHO_BLACK;
				moved = true;
			}
		}
		else if (grid[(yCord - 1) * m_noGrid + xCord - 1] == WHO_BLACK && !black)
		{
			if (grid[(yCord - 2) * m_noGrid + xCord - 2] == WHO_WHITE)
			{
				grid[(yCord - 1) * m_noGrid + xCord - 1] = grid[yCord * m_noGrid + xCord] = WHO_WHITE;
				moved = true;
			}
		}
	}

	//Bottom Right
	if (xCord + 2 < m_noGrid && yCord - 2 > -1)
	{
		if (grid[(yCord - 1) * m_noGrid + xCord + 1] == WHO_WHITE && black)
		{
			if (grid[(yCord - 2) * m_noGrid + xCord + 2] == WHO_BLACK)
			{
				grid[(yCord - 1) * m_noGrid + xCord + 1] = grid[yCord * m_noGrid + xCord] = WHO_BLACK;
				moved = true;
			}
		}
		else if (grid[(yCord - 1) * m_noGrid + xCord + 1] == WHO_BLACK && !black)
		{
			if (grid[(yCord - 2) * m_noGrid + xCord + 2] == WHO_WHITE)
			{
				grid[(yCord - 1) * m_noGrid + xCord + 1] = grid[yCord * m_noGrid + xCord] = WHO_WHITE;
				moved = true;
			}
		}
	}
	if (moved)
		return true;
	else
		return false;
}

void SceneReversi::Update(double dt)
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
		Reset();
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
		if (posX > 0 && posX < m_noGrid * m_gridSize && posY > 0 && posY < m_noGrid * m_gridSize)
		{
			int gridX = static_cast<int>(posX / m_gridSize);
			int gridY = static_cast<int>(posY / m_gridSize);
			int index = gridY * m_noGrid + gridX;
			//Exercise: Game Control
			if (!m_bGameOver)
			{
				if (m_grid[index] == WHO_NONE)
				{
					if (m_bBlackTurn)
					{
						if (Move(m_grid, m_bBlackTurn, index))
							m_bBlackTurn = false;
					}
					else //Exercise: For AI to take over
					{
						if (Move(m_grid, m_bBlackTurn, index))
							m_bBlackTurn = true;
					}
					//Game Logic Section - call methods
					//Exercise: Flips the existing disks on board
					//Exercise: Check if there is valid move for current color, if not forfeit the turn 
					//Exercise: Check draw, black wins or white wins
				}
			}
		}
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

	if (!m_bGameOver)
	{
		//Exercise: For AI to take over white player (or both players)
	}
}

void SceneReversi::Render()
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

	//Exercise: Render Game board
	//Exercise: Render black and white seeds without GameObject


	modelStack.PushMatrix();
	modelStack.Translate(m_worldHeight * 0.5f, m_worldHeight * 0.5f, 0.f);
	modelStack.Scale(m_worldHeight, m_worldHeight, 0);
	RenderMesh(meshList[GEO_REVERSI], false);
	modelStack.PopMatrix();

	int black, white;
	black = white = 0;
	for (int x = 0; x < m_grid.size(); ++x)
	{
		if (m_grid[x] == WHO_NONE)
			continue;
		modelStack.PushMatrix();
		modelStack.Translate((x % m_noGrid) * m_gridSize + m_gridOffset, (x / m_noGrid) * m_gridSize + m_gridOffset, 1);
		modelStack.Scale(m_gridSize, m_gridSize, 0);
		switch (m_grid[x])
		{
		case WHO_BLACK:
			RenderMesh(meshList[GEO_BLACK], false);
			++black;
			break;
		case WHO_WHITE:
			RenderMesh(meshList[GEO_WHITE], false);
			++white;
			break;
		}
		modelStack.PopMatrix();
	}

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
	ss.precision(5);
	ss << "Whites:" << white;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss.precision(5);
	ss << "Blacks:" << black;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);

	ss.str("");
	ss.precision(5);
	if (m_bBlackTurn)
		ss << "Turn: Black";
	else
		ss << "Turn: White";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 15);

	if (m_bGameOver)
	{
		ss.str("");
		ss.precision(5);
		if (black == white)
			ss << "Draw";
		else if (black > white)
			ss << "Black Wins!";
		else
			ss << "White Wins!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 18);
	}

	//Exercise: Render number of black and white seeds
	//Exercise: Render whose turn (black or white)
	//Exercise: [When gameover] Render black wins, white wins or game draw
	ss.str("");
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	RenderTextOnScreen(meshList[GEO_TEXT], "Reversi (R to reset)", Color(0, 1, 0), 3, 50, 0);
}

void SceneReversi::Exit()
{
	SceneBase::Exit();
}
