#include "stdafx.h"
#include "MoveStatic.h"

#include "BmpMgr.h"


CMoveStatic::CMoveStatic()
{
}


CMoveStatic::~CMoveStatic()
{
	Release();
}

void CMoveStatic::Initialize()
{
}

int CMoveStatic::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fY += g_fBackgroundSpeed * 3.f;			// 배경에 맞춰서 Y축 이동

	if (m_tInfo.fY > m_fDestPosY)
	{
		m_fDestPosY += g_fBackgroundSpeed * 3.f;		// 목적지도 배경 속도에 맞춰서 Y축 이동해야됨

		float	fX = m_fDestPosX - m_tInfo.fX;
		float	fY = m_fDestPosY - m_tInfo.fY;
		float	fDia = sqrtf(fX * fX + fY * fY);

		// 목적지에 도착하지 않았으면 이동 처리
		if (fDia >= m_fSpeed)
		{
			float fRad = acosf(fX / fDia);

			float fAngle = fRad * 180.f / PI;

			if (m_tInfo.fY < m_fDestPosY)
				fAngle *= -1.f;

			m_tInfo.fX += cosf(fAngle * PI / 180.f) * m_fSpeed;
			m_tInfo.fY -= sinf(fAngle * PI / 180.f) * m_fSpeed;
		}
	}

	Update_RectEx();

	return OBJ_NOEVENT;
}

void CMoveStatic::Late_Update()
{
	if (WINCY <= m_tRect.top || WINCX <= m_tRect.left)
		m_bDead = true;
}

void CMoveStatic::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC
		, (int)m_tInfo.fX, (int)m_tInfo.fY			// 중점좌표없이 left top을 x y 로 봄
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CMoveStatic::Release()
{
}

void CMoveStatic::Update_RectEx()
{
	m_tRect.left = (LONG)m_tInfo.fX;
	m_tRect.top = (LONG)m_tInfo.fY;
	m_tRect.right = (LONG)(m_tInfo.fX + m_tInfo.iCX);
	m_tRect.bottom = (LONG)(m_tInfo.fY + m_tInfo.iCY);
}
