#include "SceneQueen.h"
#include "SceneQueen.h"
#include "SceneQueen.h"
#include "SceneQueen.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneQueen::SceneQueen()
{
}

SceneQueen::~SceneQueen()
{
}

void SceneQueen::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;
	m_noGrid = 8;
	m_gridSize = m_worldHeight / m_noGrid;
	m_gridOffset = m_gridSize / 2;

	m_queen.resize(8, -1);

	m_numSolutions = 0;
	DFS(0);
	std::cout << "Num solutions: " << m_numSolutions << std::endl;
}

void SceneQueen::Update(double dt)
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

void SceneQueen::Render()
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
	RenderMesh(meshList[GEO_CHESSBOARD], false);
	modelStack.PopMatrix();

	//Print queens

	//On screen text
	std::ostringstream ss;
	//ss << "Something" << something;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	//ss << "Something" << something;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	//ss << "Something" << something;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	RenderTextOnScreen(meshList[GEO_TEXT], "8 Queens", Color(0, 1, 0), 3, 50, 0);

	for (int i = 0; i < m_queen.size(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate((m_queen[i] + 0.5f) * m_gridSize, (i + 0.5f) * m_gridSize, 5.f);
		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
		RenderMesh(meshList[GEO_QUEEN], false);
		modelStack.PopMatrix();
	}
}

void SceneQueen::Exit()
{
	SceneBase::Exit();
}

void SceneQueen::PrintSolution()
{
	std::cout << "Solution:";
	for (int x = 0; x < m_queen.size(); ++x)
		std::cout << " " << m_queen[x];
	std::cout << std::endl;
}

bool SceneQueen::CheckQueen()
{
	for (int x = 0; x < m_noGrid; ++x)
	{
		for (int y = x + 1; y < m_noGrid; ++y)
		{
			if (m_queen[y] == -1)
				continue;
			if (x != y)
			{
				if (m_queen[x] == m_queen[y])
					return false;
				if (m_queen[y] == m_queen[x] + 1 * abs(x - y) || m_queen[y] == m_queen[x] - 1 * abs(x - y))
					return false;
			}
		}
	}

	return true;
}

void SceneQueen::DFS(int row)
{
	Application::GetInstance().Iterate();
	for (int x = 0; x < m_noGrid; ++x)
	{
		m_queen[row] = x;
		if (CheckQueen())
		{

			if (row != m_noGrid - 1)
			{
				DFS(row + 1);
			}
			else
			{
				++m_numSolutions;
				PrintSolution();
				Application::GetInstance().Iterate();
				Sleep(1000);
			}
		}
		m_queen[row] = -1;
	}
}
