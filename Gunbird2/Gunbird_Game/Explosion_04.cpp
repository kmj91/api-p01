#include "stdafx.h"
#include "Explosion_04.h"

#include "BmpMgr.h"
#include "SoundMgr.h"


CExplosion_04::CExplosion_04()
{
}


CExplosion_04::~CExplosion_04()
{
	Release();
}

void CExplosion_04::Initialize()
{
	m_tInfo.iCX = EXPLOSION_04_WIDTH * 3;
	m_tInfo.iCY = EXPLOSION_04_HEIGHT * 3;
	m_iImageWidth = EXPLOSION_04_WIDTH;
	m_iImageHeight = EXPLOSION_04_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 19;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

int CExplosion_04::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_dwEffectTime + m_dwEffectDelay > GetTickCount())		// ÀÌÆåÆ® µô·¹ÀÌ
		return OBJ_NOEVENT;
	else
	{
		if (!m_bOnEffect)
		{
			m_bOnEffect = true;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EXPLOSION_4);
			CSoundMgr::Get_Instance()->PlaySound(L"Explosion_04.wav", CSoundMgr::EXPLOSION_4);
		}
	}

	Update_Rect();
	Frame_Move();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)		// ÇÁ·¹ÀÓÀÌ ³¡ÀÌ¸é »ç¶óÁü
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CExplosion_04::Late_Update()
{
}

void CExplosion_04::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Explosion_04");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CExplosion_04::Release()
{
}
