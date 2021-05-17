#include "stdafx.h"
#include "BulletHit.h"

#include  "BmpMgr.h"


CBulletHit::CBulletHit()
{
}


CBulletHit::~CBulletHit()
{
	Release();
}

void CBulletHit::Initialize()
{
	m_tInfo.iCX = BULLET_HIT_WIDTH * 3;
	m_tInfo.iCY = BULLET_HIT_HEIGHT * 3;
	m_iImageWidth = BULLET_HIT_WIDTH;
	m_iImageHeight = BULLET_HIT_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 17;
	m_tFrame.iFrameScene = rand() % 2;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 10;
}

int CBulletHit::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)		// 프레임이 끝이면 사라짐
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CBulletHit::Late_Update()
{
	Update_Rect();
}

void CBulletHit::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BulletHit");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CBulletHit::Release()
{
}
