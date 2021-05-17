#include "stdafx.h"
#include "UI_PowerUpDownText.h"

#include  "BmpMgr.h"


CUI_PowerUpDownText::CUI_PowerUpDownText()
	: m_dwTime(GetTickCount()), m_dwDelay(600)
{
}


CUI_PowerUpDownText::~CUI_PowerUpDownText()
{
	Release();
}

void CUI_PowerUpDownText::Initialize()
{
	m_tInfo.iCX = POWER_TEXT_WIDTH * 3;
	m_tInfo.iCY = POWER_TEXT_HEIGHT * 3;
	m_iImageWidth = POWER_TEXT_WIDTH;
	m_iImageHeight = POWER_TEXT_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 150;

	m_fSpeed = 3.0f;
}

int CUI_PowerUpDownText::Update()
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

void CUI_PowerUpDownText::Late_Update()
{
}

void CUI_PowerUpDownText::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_PowerUpDownText");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CUI_PowerUpDownText::Release()
{
}
