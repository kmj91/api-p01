#include "stdafx.h"
#include "Obj.h"

CObj::CObj()
	: m_tInfo{}, m_tRect{}, m_tHitRectPos{}, m_tHitRect{}, m_tFrame{}, m_fSpeed(0.f), m_fAngle(0.f), m_pTarget(nullptr), m_pFrameKey(nullptr),
	m_iDrawID(0), m_iImageWidth(0), m_iImageHeight(0), m_fHp(1.f), m_bDead(false), m_bHpLock(true), m_bRemove(false)
{
}

CObj::~CObj()
{
}

void CObj::Hit(float _fDamage)
{
	m_fHp -= _fDamage;
	if (m_fHp <= 0.f)
		m_bDead = true;
}

void CObj::Update_Rect()
{
	LONG lCX = LONG(m_tInfo.iCX >> 1);
	LONG lCY = LONG(m_tInfo.iCY >> 1);

	m_tRect.left = (LONG)m_tInfo.fX - lCX;
	m_tRect.top = (LONG)m_tInfo.fY - lCY;
	m_tRect.right = (LONG)m_tInfo.fX + lCX;
	m_tRect.bottom = (LONG)m_tInfo.fY + lCY;

	LONG lX = (LONG)m_tInfo.fX - lCX;
	LONG lY = (LONG)m_tInfo.fY - lCY;

	m_tHitRect.left = lX + m_tHitRectPos.left;
	m_tHitRect.top = lY + m_tHitRectPos.top;
	m_tHitRect.right = lX + m_tHitRectPos.right;
	m_tHitRect.bottom = lY + m_tHitRectPos.bottom;
}

void CObj::Frame_Move()
{
	if (m_tFrame.iFrameStart <= m_tFrame.iFrameEnd)
	{
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			++m_tFrame.iFrameCnt;
			if (m_tFrame.iFrameCnt > m_tFrame.iFrameEnd)
				m_tFrame.iFrameCnt = m_tFrame.iFrameStart;

			m_tFrame.dwFrameTime = GetTickCount();
		}
	}
	else
	{
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			--m_tFrame.iFrameCnt;
			if (m_tFrame.iFrameCnt < m_tFrame.iFrameEnd)
				m_tFrame.iFrameCnt = m_tFrame.iFrameStart;

			m_tFrame.dwFrameTime = GetTickCount();
		}
	}

	
}
