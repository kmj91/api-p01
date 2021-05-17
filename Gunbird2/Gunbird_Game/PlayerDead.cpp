#include "stdafx.h"
#include "PlayerDead.h"

#include "BmpMgr.h"


CPlayerDead::CPlayerDead()
{
}


CPlayerDead::~CPlayerDead()
{
	Release();
}

void CPlayerDead::Initialize()
{
	m_tInfo.iCX = PLAYER_DEAD_WIDTH * 3;
	m_tInfo.iCY = PLAYER_DEAD_HEIGHT * 3;
	m_iImageWidth = PLAYER_DEAD_WIDTH;
	m_iImageHeight = PLAYER_DEAD_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

int CPlayerDead::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_dwEffectTime + m_dwEffectDelay > GetTickCount())		// ÀÌÆåÆ® µô·¹ÀÌ
		return OBJ_NOEVENT;
	else
		m_bOnEffect = true;

	Update_Rect();
	Frame_Move();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)		// ÇÁ·¹ÀÓÀÌ ³¡ÀÌ¸é »ç¶óÁü
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CPlayerDead::Late_Update()
{
}

void CPlayerDead::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PlayerDead");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CPlayerDead::Release()
{
}
