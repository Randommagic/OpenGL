
// GraphicsProjectView.cpp: CGraphicsProjectView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GraphicsProject.h"
#endif

#include "GraphicsProjectDoc.h"
#include "GraphicsProjectView.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphicsProjectView

IMPLEMENT_DYNCREATE(CGraphicsProjectView, CView)

BEGIN_MESSAGE_MAP(CGraphicsProjectView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGraphicsProjectView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGraphicsProjectView 생성/소멸

CGraphicsProjectView::CGraphicsProjectView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CGraphicsProjectView::~CGraphicsProjectView()
{
}

BOOL CGraphicsProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CGraphicsProjectView 그리기

void CGraphicsProjectView::OnDraw(CDC* /*pDC*/)
{
	CGraphicsProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	DrawGLScene();
	Invalidate(FALSE);
}


// CGraphicsProjectView 인쇄


void CGraphicsProjectView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGraphicsProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CGraphicsProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CGraphicsProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CGraphicsProjectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGraphicsProjectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGraphicsProjectView 진단

#ifdef _DEBUG
void CGraphicsProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicsProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicsProjectDoc* CGraphicsProjectView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicsProjectDoc)));
	return (CGraphicsProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphicsProjectView 메시지 처리기
BOOL CGraphicsProjectView::SetDevicePixelFormat(HDC hdc)
{
	int pixelformat;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                              // default version
		PFD_DRAW_TO_WINDOW |            // support window 
		PFD_SUPPORT_OPENGL |            // support OpenGL 
		PFD_GENERIC_FORMAT |
		PFD_DOUBLEBUFFER,               // double buffered 
		PFD_TYPE_RGBA,                  // RGBA type 
		32,                             // 32-bit color depth 
		0, 0, 0, 0, 0, 0,               // color bits ignored 
		8,                              // no alpha buffer 
		0,                              // shift bit ignored 
		8,                              // no accumulation buffer 
		0, 0, 0, 0,                     // accum bits ignored 
		16,                             // 16-bit z-buffer     
		0,                              // no stencil buffer 
		0,                              // no auxiliary buffer 
		PFD_MAIN_PLANE,                 // main layer 
		0,                              // reserved 
		0, 0, 0                         // layer masks ignored 
	};
	// choose best matching pixel format
	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE) {
		MessageBox(LPCTSTR("ChoosePixelFormat failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}
	// set pixel format to device context
	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) {
		MessageBox(LPCTSTR("SetPixelFormat failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}
	return TRUE;
}


void CGraphicsProjectView::InitGL(void)
{

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // 앞의 물체에 가린 면은 보이지 않음.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	m_sokoban.initialize();
	BuildFont();
	
}



void CGraphicsProjectView::ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CGraphicsProjectView::DrawGLScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	m_sokoban.DrawGLScene();

	Drawtext(m_sokoban.status);


	SwapBuffers(m_hDC);

}

void CGraphicsProjectView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ReSizeGLScene(cx, cy);
}


int CGraphicsProjectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	m_hDC = GetDC()->m_hDC;

	if (!SetDevicePixelFormat(m_hDC)) {
		return -1;
	}

	m_hgIRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hgIRC);
	InitGL();

	return 0;
}


void CGraphicsProjectView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	wglMakeCurrent(m_hDC, NULL);
	wglDeleteContext(m_hgIRC);

	KillFont();
	m_sokoban.destroy();
}


void CGraphicsProjectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_sokoban.b_mouseMove = TRUE;
	m_sokoban.m_anchorPoint = point;
	CView::OnLButtonDown(nFlags, point);
}


void CGraphicsProjectView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_sokoban.m_currentPoint = point;
	CView::OnMouseMove(nFlags, point);
}


void CGraphicsProjectView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_sokoban.b_mouseMove = FALSE;
	CView::OnLButtonUp(nFlags, point);
}


void CGraphicsProjectView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar) {
	case VK_UP:
		 m_sokoban.Move(0, -1);
		 m_sokoban.direction = 2;
		break;
	case VK_DOWN:
		m_sokoban.Move(0, 1);
		m_sokoban.direction = 0;
		break;
	case VK_LEFT:
		 m_sokoban.Move(-1, 0);
		 m_sokoban.direction = 3;
		break;
	case VK_RIGHT:
		 m_sokoban.Move(1, 0);
		 m_sokoban.direction = 1;
		break;

	case 'R':
		m_sokoban.RestartMap();
		break;

	case 'S':
		if (m_sokoban.status != INGAME)  m_sokoban.status = INGAME;
		break;

	case 'N':
		if (m_sokoban.status == TITLE || m_sokoban.level == 4) break;
		m_sokoban.status = INGAME;
		m_sokoban.level++;
		m_sokoban.LoadMapFile();
		m_sokoban.RestartMap();
		break;

	case 'P':
		if (m_sokoban.status == TITLE || m_sokoban.level == 0) break;
		m_sokoban.status = INGAME;
		m_sokoban.level--;
		m_sokoban.LoadMapFile();
		m_sokoban.RestartMap();
		break;

	}



	

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

GLvoid CGraphicsProjectView::BuildFont() {

	fontsize = 0;

	base = glGenLists(96);
	titlefont = CreateFont(-100, 
		0,
		0,
		0,
		FW_BOLD, 
		FALSE,
		FALSE, 
		FALSE,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		_T("Courier New"));

	ingamefont = CreateFont(-24,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET, 
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		_T("Courier New"));

	font2 = CreateFont(-75,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		_T("Courier New"));

}

GLvoid  CGraphicsProjectView::KillFont(GLvoid) {
	glDeleteLists(base, 96);
}

GLvoid CGraphicsProjectView::glPrint(const char* text) {
	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}
void CGraphicsProjectView::Drawtext(int k) {
	if (k == TITLE) {
		SelectObject(m_hDC, titlefont);
		wglUseFontBitmaps(m_hDC, 32, 96, base);
		char buffer[64] = "SOKOBAN GUYS";
		char buffer2[64] = "Press S to start!";
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos3f(-20.0f, 45.0f, 20.0f);
		glPrint(buffer);

		SelectObject(m_hDC, ingamefont);
		wglUseFontBitmaps(m_hDC, 32, 96, base);
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos3f(0.0f, 45.0f, 25.0f);
		glPrint(buffer2);
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else if(k == INGAME) {
		SelectObject(m_hDC, ingamefont);
		wglUseFontBitmaps(m_hDC, 32, 96, base);


		char buffer[16];
		char buffer2[64] = "Press R to restart ";
		char buffer3[64] = "Level:     ";
		char buffer4[64] = "Steps:     ";
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(-45.0f, 45.0f, 30.0f);
		glPrint(buffer2);

		glRasterPos3f(-46.0f, 43.0f, 30.0f);
		_itoa(m_sokoban.level, buffer, 10);
		strcat(buffer3, buffer);
		glPrint(buffer3);

		glRasterPos3f(-47.4f, 41.0f, 30.0f);
		_itoa(m_sokoban.steps, buffer, 10);
		strcat(buffer4, buffer);
		glPrint(buffer4);
	}
	else{
		SelectObject(m_hDC, ingamefont);
		wglUseFontBitmaps(m_hDC, 32, 96, base);
		char buffer[64] = "SOKOBAN clear~";
		char buffer2[64] = "N : Next Level / P : Prev Level";
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos3f(-45.0f, 25.0f, -10.0f);
		glPrint(buffer);

		glRasterPos3f(-47.0f, 23.0f, -10.0f);
		glPrint(buffer2);

	}
}