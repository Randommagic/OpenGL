#pragma once
#include <gl\gl.H>
#include <gl\glu.H>
#define TITLE 1
#define INGAME 2
#define CLEAR 3

class Sokoban
{
public:

	CPoint      m_anchorPoint, m_currentPoint;
	BOOL      b_mouseMove;
	const GLfloat angleVelocity = 0.5;

	GLint map_load[8][8];
	int status, level;

	int crownrot = 0;
	int ballrot = 0;
	int p_x, p_y, targetno, currentno, steps;
	int list[7];

	GLUquadricObj* obj;
	int direction;

public:
	void initialize();
	void destroy();



	void DrawGLScene();

	void BuildLists();


	void LoadMapFile();
	void RenderMap();
	void RenderClear();

	void RenderGround();

	void RenderBoxSurface(int i);
	void RenderBox(int i);
	void Renderball();

	void StartGame();
	void RestartMap();


	void Move(int x, int y);

	
	void RenderPlayer();
	void crown();
	void crown2();

};
