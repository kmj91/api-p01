#include "stdafx.h"
#include "Rocket.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "Explosion_01.h"


CRocket::CRocket()
{
}


CRocket::~CRocket()
{
	Release();
}

void CRocket::Initialize()
{
	m_tInfo.iCX = ROCKET_WIDTH * 3;
	m_tInfo.iCY = ROCKET_HEIGHT * 3;
	m_tHitRectPos = { 1 * 3, 5 * 3, 20 * 3, 30 * 3 };
	m_iImageWidth = ROCKET_WIDTH;
	m_iImageHeight = ROCKET_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 20;

	m_fSpeed = 6.0f;

	// 점수
	m_iScore = 200;
}

int CRocket::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}

	m_tInfo.fY += m_fSpeed;

	Update_Rect();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CRocket::Late_Update()
{
	if (m_bHpLock)
		if (0 <= m_tRect.top)
			m_bHpLock = false;

	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

void CRocket::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Rocket");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 충돌 박스
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CRocket::Release()
{
}
