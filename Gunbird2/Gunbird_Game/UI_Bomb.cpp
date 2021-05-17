#include "stdafx.h"
#include "UI_Bomb.h"

#include "BmpMgr.h"


CUI_Bomb::CUI_Bomb() : m_iBombCnt(0)
{
}


CUI_Bomb::~CUI_Bomb()
{
	Release();
}

void CUI_Bomb::Initialize()
{
	m_tInfo.iCX = UI_BOMB_WIDTH * 3;
	m_tInfo.iCY = UI_BOMB_HEIGHT * 3;
	m_iImageWidth = UI_BOMB_WIDTH;
	m_iImageHeight = UI_BOMB_HEIGHT;
}

int CUI_Bomb::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CUI_Bomb::Late_Update()
{
}

void CUI_Bomb::Render(HDC _DC)
{
	if (m_iBombCnt == 0) {
		return;
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_BombNum");

	GdiTransparentBlt(_DC
		, m_tInfo.fX, m_tInfo.fY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, (m_iBombCnt - 1) * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CUI_Bomb::Release()
{
}
