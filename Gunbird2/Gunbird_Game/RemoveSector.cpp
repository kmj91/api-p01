#include "stdafx.h"
#include "RemoveSector.h"


CRemoveSector::CRemoveSector()
{
}


CRemoveSector::~CRemoveSector()
{
	Release();
}

void CRemoveSector::Initialize()
{
}

int CRemoveSector::Update()
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	if (!g_bStopMapScroll)
		m_tInfo.fY += g_fBackgroundSpeed * 3.f;			// 배경에 맞춰서 Y축 이동

	Update_Rect();

	return OBJ_NOEVENT;
}

void CRemoveSector::Late_Update()
{
	if (WINCY <= m_tRect.top)
		m_bDead = true;
}

void CRemoveSector::Render(HDC _DC)
{
	// 충돌 박스
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CRemoveSector::Release()
{
}
