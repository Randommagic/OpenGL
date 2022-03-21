#include "pch.h"
#include "Sokoban.h"




void Sokoban::initialize() {
	glShadeModel(GL_SMOOTH);

	BuildLists();
	
	status = TITLE;
	targetno = currentno = 0;
	level = 0;
	steps = 0;
	b_mouseMove = FALSE;
	direction = 0; //밑쪽 바라보게
	//
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_FILL); // 채워져 있는 도형
	gluQuadricNormals(obj, GLU_SMOOTH); // 법선 벡터 설정
	gluQuadricOrientation(obj, GLU_OUTSIDE); // 법선백터의 위치 - 바깥쪽
	gluQuadricTexture(obj, GL_TRUE); // 텍스쳐의 유무
	
	//
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	LoadMapFile();

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glShadeModel(GL_SMOOTH);
	
	
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light0_position[] = { 0.0f, -50.0f, 0.0f, 0.0 };
	GLfloat light0_ambientcolor[] = { 0.5f,0.5f, 0.5f ,1.0 };
	GLfloat light0_diffusecolor[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light0_specularcolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambientcolor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffusecolor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specularcolor);

}

void Sokoban::destroy() {
	gluDeleteQuadric(obj);
}


void Sokoban::DrawGLScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	if(status !=CLEAR) gluLookAt(0.0f, 90.0f, 60.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	else 	gluLookAt(0.0f, 25.0f, 50.f, 0.f, 16.f, 0.f, 0.f, 1.f, 0.f);

	if (b_mouseMove) {
		glRotatef(angleVelocity * (m_currentPoint.x - m_anchorPoint.x), 0.0, 1.0, 0.0);
		glRotatef(angleVelocity * (m_currentPoint.y - m_anchorPoint.y), 1.0, 0.0, 0.0);
	}

	

	if (status == TITLE) {
		glPushMatrix();
		glTranslatef(-7.0, -7.0, 0);
		glRotatef(-50,1.0,0.0,0.0);
		RenderPlayer();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(5.0, 0, 0);
		glRotatef(ballrot, 1.0, 1.0, 1.0);
		Renderball();
		ballrot += 10;
		glPopMatrix();
	}	
	else if (status == INGAME) {
		RenderGround();
		RenderMap();
	}
	else {
	
		RenderClear();
		RenderGround();
	}

	


	if (targetno == currentno) status = CLEAR;

	
	
	glFlush();
}



void Sokoban::BuildLists() {
	
	list[0] = glGenLists(1);
	list[1] = glGenLists(2);
	list[2] = glGenLists(3);
	list[3] = glGenLists(4);
	list[4] = glGenLists(5);
	list[5] = glGenLists(6);
	list[6] = glGenLists(7);

	glNewList(list[0], GL_COMPILE);
	RenderBox(1);
	glEndList();

	glNewList(list[1], GL_COMPILE);
	RenderBox(2);

	glEndList();

	glNewList(list[2], GL_COMPILE);
	//unused
	glEndList();

	glNewList(list[3], GL_COMPILE);
	RenderBox(4);
	glEndList();

	glNewList(list[4], GL_COMPILE);
	//unused
	glEndList();

	glNewList(list[5], GL_COMPILE);
	//unused
	glEndList();

	glNewList(list[6], GL_COMPILE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	crown();
	crown2();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEndList();

}


GLint map_levels[5][8][8] = {
	 

	{
		{1,1,1,1,1,1,1,1},
		{1,4,4,0,0,0,0,1},
		{1,0,0,0,3,3,0,1},
		{1,0,0,4,1,0,0,1},
		{1,0,0,0,1,0,1,1},
		{1,1,1,3,0,0,0,1},
		{0,0,1,0,2,0,0,1},
		{0,0,1,1,1,1,1,1}
	},
	{
		{0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,0},
		{1,4,0,0,0,0,1,0},
		{1,3,5,0,1,0,1,0},
		{1,4,0,0,3,5,1,0},
		{1,0,4,3,0,0,1,0},
		{1,2,0,5,0,0,1,0},
		{1,1,1,1,1,1,1,0}
	},
	{
		{0,0,1,1,1,1,0,0},
		{0,1,1,0,0,1,0,0},
		{0,1,0,2,3,1,0,0},
		{0,1,1,3,0,1,1,0},
		{0,1,1,0,3,0,1,0},
		{0,1,4,3,0,0,1,0},
		{0,1,4,4,5,4,1,0},
		{0,1,1,1,1,1,1,0}
	},
	{
		{0,1,1,1,1,0,0,0},
		{0,1,2,0,1,1,0,0},
		{0,1,0,3,0,0,1,0},
		{1,1,1,0,1,0,1,1},
		{1,4,1,0,1,0,0,1},
		{1,4,3,0,0,0,0,1},
		{1,4,0,0,0,3,0,1},
		{1,1,1,1,1,1,1,1}
	},
	{
		{1,1,1,1,1,1,1,0},
		{1,4,4,3,4,4,1,0},
		{1,4,4,1,4,4,1,0},
		{1,0,3,3,3,0,1,0},
		{1,0,0,3,0,0,1,0},
		{1,0,3,3,3,2,1,0},
		{1,0,0,1,0,0,1,0},
		{1,1,1,1,1,1,1,0}
	}


};

void Sokoban::LoadMapFile() {
	currentno = 0;
	targetno = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			map_load[i][j] = map_levels[level][i][j];
			if (map_load[i][j] == 2) {
				p_x = j;
				p_y = i;
			}
			else if (map_load[i][j] == 4 || map_load[i][j] == 6) {
				targetno++;
			}
			else if (map_load[i][j] == 5) {
				currentno++;
				targetno++;
			}

			
		}
	}

}

void Sokoban::RenderMap() {
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (map_load[i][j] == 1) {

				glPushMatrix();
				glTranslatef(-35+10*j, 5.1, 10*i-35);
				if ((i + j) % 2 == 0) 	glCallList(list[0]);
				else glCallList(list[1]);
  
				glPopMatrix();
			}
			else if (map_load[i][j] == 2) {

				glPushMatrix();
				glTranslatef(-35 + 10 * j, 0.1, 10 * i - 35);
				glRotatef(direction * 90, 0.0f, 1.0f, 0.0f);
				RenderPlayer();
				glPopMatrix();
			}
			else if (map_load[i][j] == 3) {

				glPushMatrix();
				glTranslatef(-35 + 10.0 * j, 5.1, 10.0 * i - 35);
				glColor4f(0.8f, 0.8f, 1.0f, 1.0f);
				Renderball();

				glPopMatrix();
			}
			else if (map_load[i][j] == 4) {

				glPushMatrix();
				glTranslatef(-35.0 + 10.0 * j, 5.1, 10.0 * i - 35.0);
				glCallList(list[3]);
				glPopMatrix();
			}
			else if (map_load[i][j] == 5) {

				glPushMatrix();
				glTranslatef(-35.0 + 10.0 * j, 5.1, 10.0 * i - 35.0);
				glColor4f(1.0f, 1.0f, 0.5f, 1.0f);
				Renderball();
				glCallList(list[3]);
				
				glPopMatrix();
			}
			else if (map_load[i][j] == 6) {

				glPushMatrix();
				glTranslatef(-35.0 + 10.0 * j, 5.1, 10.0 * i - 35.0);
				glCallList(list[3]);
				glPopMatrix();
				glPushMatrix();
				glTranslatef(-35.0 + 10.0 * j, 2.1, 10.0 * i - 35.0);
				glRotatef(direction * 90, 0.0f, 1.0f, 0.0f);
				RenderPlayer();
				glPopMatrix();

				

			}
		}
	}

}

void Sokoban::RenderClear() {
	CImage  image;
	BITMAP  BM;
	HBITMAP  hBmp;
	CString fname("textures\\win1.bmp");
	image.Load((LPCTSTR)fname);
	hBmp = (HBITMAP)image;
	::GetObject(hBmp, sizeof(BM), &BM);


	glTexImage2D(GL_TEXTURE_2D, 0, 3, BM.bmWidth, BM.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BM.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL

	glEnable(GL_TEXTURE_2D);


	glPushMatrix();

	
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-40.0, 0.0, -40.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(40.0, 0.0, -40.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(40.0, 60.0, -40.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-40.0, 60.0, -40.0);
	glEnd();
	
	glPopMatrix();





	glPushMatrix();
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	gluCylinder(obj, 15.0f, 15.0f, 13.0f, 16, 4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 13.0f, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluDisk(obj, 0.0f, 15.0f, 16, 4);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
		
	glPushMatrix();
	glTranslatef(0.0f, 13.0f, 0.0f);
	RenderPlayer();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 27.0f, 0.0f);
	glRotatef(crownrot, 0.0f, 1.0f, 0.0f);
	glCallList(list[6]);
	crownrot += 10;
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void Sokoban::RenderBoxSurface(int i) {
	CImage  image;
	BITMAP  BM;
	HBITMAP  hBmp;
	CString file;
	switch(i) {
	case 1: //벽
		file = ("textures\\wall1.bmp");
		break;
	case 2:
		file = ("textures\\wall2.bmp");
		break;
	case 4:
		file = ("textures\\house.bmp");
		break;
	}

	
	
	image.Load((LPCTSTR)file);
	hBmp = (HBITMAP)image;
	::GetObject(hBmp, sizeof(BM), &BM);
	glEnable(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BM.bmWidth, BM.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BM.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST,GL_LINEAR
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // GL_DECAL

	
	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 1);
	if (i == 4) {
		glColor4f(1, 1, 1, 0.7);
	}
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-5.0, 5.0, -5.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(5.0, 5.0, -5.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(5.0, 5.0, 5.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-5.0, 5.0, 5.0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, 1);
}

void Sokoban::RenderBox(int i) {
	
	glPushMatrix();
	RenderBoxSurface(i);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	RenderBoxSurface(i);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	RenderBoxSurface(i);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	RenderBoxSurface(i);
	glPopMatrix();
	
		
	glPushMatrix(); 
	glRotatef(90.0, 1.0, 0.0, 0.0);

	RenderBoxSurface(i);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	RenderBoxSurface(i);
	glPopMatrix();

}

void Sokoban::RenderGround() {


	CImage  image;
	BITMAP  BM;
	HBITMAP  hBmp;
	CString fname("textures\\floor.bmp");
	image.Load((LPCTSTR)fname);
	hBmp = (HBITMAP)image;
	::GetObject(hBmp, sizeof(BM), &BM);


	glTexImage2D(GL_TEXTURE_2D, 0, 3, BM.bmWidth, BM.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BM.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL


	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	//glNormal3f(.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-40.0, 0.0, -40.0);

	glTexCoord2f(3.0, 0.0);
	glVertex3f(40.0, 0.0, -40.0);

	glTexCoord2f(3.0, 3.0);
	glVertex3f(40.0, 0.0, 40.0);

	glTexCoord2f(0.0, 3.0);
	glVertex3f(-40.0, 0.0, 40.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}



void Sokoban::Move(int x, int y) {
	if (status != INGAME) return;
	int moved = 0;

	if (map_load[p_y][p_x] == 2) {
		if (map_load[p_y + y][p_x + x] == 0) {      //밀 공간이 빈공간
			map_load[p_y][p_x] = 0;
			map_load[p_y + y][p_x + x] = 2;
			moved = 1;
		}
		else if (map_load[p_y + y][p_x + x] == 3) {  //밀 공간이 폭탄
			if (map_load[p_y + 2 * y][p_x + 2 * x] == 0) {
				map_load[p_y][p_x] = 0;
				map_load[p_y + y][p_x + x] = 2;
				map_load[p_y + 2 * y][p_x + 2 * x] = 3;
				moved = 1;
			}
			else if (map_load[p_y + 2 * y][p_x + 2 * x] == 4) {
				map_load[p_y][p_x] = 0;
				map_load[p_y + y][p_x + x] = 2;
				map_load[p_y + 2 * y][p_x + 2 * x] = 5;
				moved = 1;
				currentno++;
			}
		}

		else if (map_load[p_y + y][p_x + x] == 4) {  //밀 공간이 빈집
			map_load[p_y][p_x] = 0;
			map_load[p_y + y][p_x + x] = 6;
			moved = 1;
		}

		else if (map_load[p_y + y][p_x + x] == 5) { //밀 공간이 폭탄집
			if (map_load[p_y + 2 * y][p_x + 2 * x] == 0) {
				map_load[p_y][p_x] = 0;
				map_load[p_y + y][p_x + x] = 6;
				map_load[p_y + 2 * y][p_x + 2 * x] = 3;
				moved = 1;
				currentno--;
			}
			else if (map_load[p_y + 2 * y][p_x + 2 * x] == 4) {
				map_load[p_y][p_x] = 0;
				map_load[p_y + y][p_x + x] = 6;
				map_load[p_y + 2 * y][p_x + 2 * x] = 5;
				moved = 1;
			}
		}
	}
	else if (map_load[p_y][p_x] == 6) {
		if (map_load[p_y + y][p_x + x] == 0) {      //밀 공간이 빈공간
			map_load[p_y][p_x] = 4;
			map_load[p_y + y][p_x + x] = 2;
			moved = 1;
		}
		else if (map_load[p_y + y][p_x + x] == 3) {  //밀 공간이 폭탄
			if (map_load[p_y + 2 * y][p_x + 2 * x] == 0) {
				map_load[p_y][p_x] = 4;
				map_load[p_y + y][p_x + x] = 2;
				map_load[p_y + 2 * y][p_x + 2 * x] = 3;
				moved = 1;
			}
			else if (map_load[p_y + 2 * y][p_x + 2 * x] == 4) {
				map_load[p_y][p_x] = 4;
				map_load[p_y + y][p_x + x] = 2;
				map_load[p_y + 2 * y][p_x + 2 * x] = 5;
				moved = 1;
				currentno++;
			}
		}

		else if (map_load[p_y + y][p_x + x] == 4) {  //밀 공간이 빈집
			map_load[p_y][p_x] = 4;
			map_load[p_y + y][p_x + x] = 6;
			moved = 1;
		}

		else if (map_load[p_y + y][p_x + x] == 5) { //밀 공간이 폭탄집
			if (map_load[p_y + 2 * y][p_x + 2 * x] == 0) {
				map_load[p_y][p_x] = 4;
				map_load[p_y + y][p_x + x] = 6;
				map_load[p_y + 2 * y][p_x + 2 * x] = 3;
				moved = 1;
				currentno--;
			}
			else if (map_load[p_y + 2 * y][p_x + 2 * x] == 4) {
				map_load[p_y][p_x] = 4;
				map_load[p_y + y][p_x + x] = 6;
				map_load[p_y + 2 * y][p_x + 2 * x] = 5;
				moved = 1;
			}
		}
	}

	if (moved == 1) {
		p_y += y;
		p_x += x;
		steps++;
	}
}

void Sokoban::StartGame() {

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);

	gluSphere(obj, 20.0f, 24, 24);
	glPopMatrix();

}

void Sokoban::RestartMap() {
	if (!steps) return;
	currentno = 0;
	targetno = 0;
	steps = 0;
	direction = 0;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			map_load[i][j] = map_levels[level][i][j];
			if (map_load[i][j] == 2) {
				p_x = j;
				p_y = i;
			}
			else if (map_load[i][j] == 4 || map_load[i][j] == 6) {
				targetno++;
			}
			else if (map_load[i][j] == 5) {
				currentno++;
				targetno++;
			}


		}
	}

}



void Sokoban::RenderPlayer() {
	CImage  image;
	BITMAP  BM;
	HBITMAP  hBmp;
	CString fname("textures\\guys.bmp");
	image.Load((LPCTSTR)fname);
	hBmp = (HBITMAP)image;
	::GetObject(hBmp, sizeof(BM), &BM);


	glTexImage2D(GL_TEXTURE_2D, 0, 3, BM.bmWidth, BM.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BM.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL
	
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	gluCylinder(obj, 5.0f, 5.0f, 13.0f, 16, 4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 13.0f, 0.0f);
	gluSphere(obj, 5.0f, 24, 24);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);

	
	glDisable(GL_TEXTURE_2D);

}

void Sokoban::Renderball() {
	CImage  image;
	BITMAP  BM;
	HBITMAP  hBmp;
	CString fname("textures\\ball.bmp");
	image.Load((LPCTSTR)fname);
	hBmp = (HBITMAP)image;
	::GetObject(hBmp, sizeof(BM), &BM);


	glTexImage2D(GL_TEXTURE_2D, 0, 3, BM.bmWidth, BM.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BM.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();


	glTexCoord2f(0.0, 0.0);
	gluSphere(obj, 4.0f, 24, 24);

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Sokoban::crown() {
	CImage  image;
	BITMAP  BM;
	HBITMAP  hBmp;
	CString fname("textures\\crown1.bmp");
	image.Load((LPCTSTR)fname);
	hBmp = (HBITMAP)image;
	::GetObject(hBmp, sizeof(BM), &BM);


	glTexImage2D(GL_TEXTURE_2D, 0, 3, BM.bmWidth, BM.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BM.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//GL_DECAL


	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-5.0, 0.0, 0.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(5.0, 0.0, 0.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(5.0, 10.0, 0.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-5.0, 10.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	










}

void Sokoban::crown2() {
	CImage  image;
	BITMAP  BM;
	HBITMAP  hBmp;
	CString fname("textures\\crown2.bmp");
	image.Load((LPCTSTR)fname);
	hBmp = (HBITMAP)image;
	::GetObject(hBmp, sizeof(BM), &BM);


	glTexImage2D(GL_TEXTURE_2D, 0, 3, BM.bmWidth, BM.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BM.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//GL_DECAL


	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-5.0, 0.0, 0.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(5.0, 0.0, 0.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(5.0, 10.0, 0.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-5.0, 10.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);


}
