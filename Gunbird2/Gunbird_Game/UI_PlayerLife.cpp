#include "stdafx.h"
#include "UI_PlayerLife.h"

#include "BmpMgr.h"


CUI_PlayerLife::CUI_PlayerLife() : m_iLife(0)
{
}


CUI_PlayerLife::~CUI_PlayerLife()
{
	Release();
}

void CUI_PlayerLife::Initialize()
{
	m_tInfo.iCX = PLAYER_LIFE_WIDTH * 3;
	m_tInfo.iCY = PLAYER_LIFE_HEIGHT * 3;
	m_iImageWidth = PLAYER_LIFE_WIDTH;
	m_iImageHeight = PLAYER_LIFE_HEIGHT;
}

int CUI_PlayerLife::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CUI_PlayerLife::Late_Update()
{
}

void CUI_PlayerLife::Render(HDC _DC)
{
	if (m_iLife == 0)
		return;

	int iX = (int)m_tInfo.fX;
	int iY = (int)m_tInfo.fY;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_PlayerLife");

	for (int iCnt = 0; iCnt < m_iLife; ++iCnt)
	{
		iX = iX + (iCnt * (PLAYER_LIFE_WIDTH * 3));

		GdiTransparentBlt(_DC
			, iX, iY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
	}
}

void CUI_PlayerLife::Release()
{
}
