#include "stdafx.h"
#include "Bomb.h"

#include "BmpMgr.h"


CBomb::CBomb() : m_fRadian(0.f)
{
}


CBomb::~CBomb()
{
	Release();
}

void CBomb::Initialize()
{
	m_tInfo.iCX = BOMB_WIDTH * 3;
	m_tInfo.iCY = BOMB_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, BOMB_WIDTH * 3, BOMB_HEIGHT * 3 };
	m_iImageWidth = BOMB_WIDTH;
	m_iImageHeight = BOMB_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;

	m_fSpeed = 4.0f;

	int iRadValue = rand() % 20;
	m_fAngle = -70.f + (float)iRadValue;
	m_fRadian = PI * m_fAngle / 180.f;

	m_enItemType = ITEM::BOMB;
}

int CBomb::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += cosf(m_fRadian) * m_fSpeed;
	m_tInfo.fY += -sinf(m_fRadian) * m_fSpeed;

	//---------------------------
	// 화면 밖을 나가지 않는다
	//---------------------------
	if (m_tInfo.fX < 0) {
		m_fRadian = PI - m_fRadian;
	}
	if (m_tInfo.fX > WINCX) {
		m_fRadian = PI - m_fRadian;
	}
	if (m_tInfo.fY < 0) {
		m_fRadian = 2 * PI - m_fRadian;
	}
	if (m_tInfo.fY > WINCY - 200) {
		m_fRadian = 2 * PI - m_fRadian;
	}


	Update_Rect();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CBomb::Late_Update()
{
}

void CBomb::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bomb");
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
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CBomb::Release()
{
}
