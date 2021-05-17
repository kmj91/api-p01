#include "stdafx.h"
#include "Explosion_05.h"

#include "BmpMgr.h"
#include "SoundMgr.h"


CExplosion_05::CExplosion_05()
{
}


CExplosion_05::~CExplosion_05()
{
	Release();
}

void CExplosion_05::Initialize()
{
	m_tInfo.iCX = EXPLOSION_05_WIDTH * 3;
	m_tInfo.iCY = EXPLOSION_05_HEIGHT * 3;
	m_iImageWidth = EXPLOSION_05_WIDTH;
	m_iImageHeight = EXPLOSION_05_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iFrameScene = rand() % 4;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;

	m_fSpeed = 15.f;
}

int CExplosion_05::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_dwEffectTime + m_dwEffectDelay > GetTickCount())		// 이펙트 딜레이
		return OBJ_NOEVENT;
	else
	{
		if (!m_bOnEffect)
		{
			m_bOnEffect = true;
		}
	}

	// 아래로 이동
	m_tInfo.fY += m_fSpeed;

	Update_Rect();
	Frame_Move();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)		// 프레임이 끝이면 사라짐
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CExplosion_05::Late_Update()
{
}

void CExplosion_05::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Explosion_05");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CExplosion_05::Release()
{
}
