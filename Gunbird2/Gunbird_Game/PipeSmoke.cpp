#include "stdafx.h"
#include "PipeSmoke.h"

#include "BmpMgr.h"


CPipeSmoke::CPipeSmoke()
{
}


CPipeSmoke::~CPipeSmoke()
{
	Release();
}

void CPipeSmoke::Initialize()
{
	m_tInfo.iCX = PIPE_SMOKE_WIDTH * 3;
	m_tInfo.iCY = PIPE_SMOKE_HEIGHT * 3;
	m_iImageWidth = PIPE_SMOKE_WIDTH;
	m_iImageHeight = PIPE_SMOKE_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 15;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

int CPipeSmoke::Update()
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

	// »ó´ë ÁÂÇ¥·Î À§Ä¡¸¦ ¸ÂÃã
	m_tInfo.fX = m_pTarget->Get_Info().fX + m_fRelativePosX;
	m_tInfo.fY = m_pTarget->Get_Info().fY + m_fRelativePosY;

	Update_Rect();
	Frame_Move();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)		// ÇÁ·¹ÀÓÀÌ ³¡ÀÌ¸é »ç¶óÁü
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CPipeSmoke::Late_Update()
{
}

void CPipeSmoke::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PipeSmoke");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CPipeSmoke::Release()
{
}
