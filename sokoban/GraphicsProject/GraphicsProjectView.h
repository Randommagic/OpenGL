﻿
// GraphicsProjectView.h: CGraphicsProjectView 클래스의 인터페이스
//
#define STB_IMAGE_IMPLEMENTATION
#pragma once
#include <gl\gl.H>
#include <gl\glu.H>
#include <gl\glut.H>
#include <gl\glaux.H>
#include "Sokoban.h"


//generate 뭐시기
#pragma comment(lib,"OPENGL32.LIB")
#pragma comment(lib,"GLAUX.LIB")
#pragma comment(lib,"GLU32.LIB")
#pragma comment(lib,"GLUT32.LIB")

#define TITLE 1
#define INGAME 2
#define CLEAR 3
#define MAXLEVEL 2
class CGraphicsProjectView : public CView
{
protected: // serialization에서만 만들어집니다.
	CGraphicsProjectView() noexcept;
	DECLARE_DYNCREATE(CGraphicsProjectView)

// 특성입니다.
public:
	CGraphicsProjectDoc* GetDocument() const;

// 작업입니다.
	HDC			m_hDC;
	HGLRC		m_hgIRC;

	Sokoban		m_sokoban;
	GLuint		base;
	char*		currentno;
	HFONT	titlefont, ingamefont, font2;
	int		fontsize = 0;
public:
	BOOL	SetDevicePixelFormat(HDC hdc);
	void	InitGL(void);
	void	ReSizeGLScene(GLsizei width, GLsizei height);
	void	DrawGLScene(void);
	//GLvoid	BuildFont(GLvoid);
	GLvoid	BuildFont();
	GLvoid  KillFont(GLvoid);
	GLvoid  glPrint(const char* text);
	void	Drawtext(int k);


// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CGraphicsProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // GraphicsProjectView.cpp의 디버그 버전
inline CGraphicsProjectDoc* CGraphicsProjectView::GetDocument() const
   { return reinterpret_cast<CGraphicsProjectDoc*>(m_pDocument); }
#endif

