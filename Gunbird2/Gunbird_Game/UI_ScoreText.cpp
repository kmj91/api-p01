#include "stdafx.h"
#include "UI_ScoreText.h"

#include  "BmpMgr.h"


CUI_ScoreText::CUI_ScoreText()
	: m_dwTime(GetTickCount()), m_dwDelay(600)
{
}


CUI_ScoreText::~CUI_ScoreText()
{
	Release();
}

void CUI_ScoreText::Initialize()
{
	m_tInfo.iCX = UI_SCORE_TEXT_WIDTH * 3;
	m_tInfo.iCY = UI_SCORE_TEXT_HEIGHT * 3;
	m_iImageWidth = UI_SCORE_TEXT_WIDTH;
	m_iImageHeight = UI_SCORE_TEXT_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 150;

	m_fSpeed = 3.0f;
}

int CUI_ScoreText::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fY -= m_fSpeed;

	Update_Rect();
	Frame_Move();

	if (m_dwTime + m_dwDelay < GetTickCount())		// 시간이 지나면 사라짐
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CUI_ScoreText::Late_Update()
{
}

void CUI_ScoreText::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_ScoreText");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CUI_ScoreText::Release()
{
}
