#include "stdafx.h"
#include "Effect_MarionMeleeHit.h"

#include "BmpMgr.h"


CEffect_MarionMeleeHit::CEffect_MarionMeleeHit()
{
}


CEffect_MarionMeleeHit::~CEffect_MarionMeleeHit()
{
	Release();
}

void CEffect_MarionMeleeHit::Initialize()
{
	m_tInfo.iCX = EFFECT_MARIONMELEEHIT_WIDTH * 3;
	m_tInfo.iCY = EFFECT_MARIONMELEEHIT_HEIGHT * 3;
	m_iImageWidth = EFFECT_MARIONMELEEHIT_WIDTH;
	m_iImageHeight = EFFECT_MARIONMELEEHIT_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

int CEffect_MarionMeleeHit::Update()
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
		}
	}

	Update_Rect();
	Frame_Move();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)		// ÇÁ·¹ÀÓÀÌ ³¡ÀÌ¸é »ç¶óÁü
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CEffect_MarionMeleeHit::Late_Update()
{
}

void CEffect_MarionMeleeHit::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Effect_MarionMeleeHit");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CEffect_MarionMeleeHit::Release()
{
}
