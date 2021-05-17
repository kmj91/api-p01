#include "stdafx.h"
#include "BulletDestroy.h"

#include  "BmpMgr.h"


CBulletDestroy::CBulletDestroy()
{
}


CBulletDestroy::~CBulletDestroy()
{
	Release();
}

void CBulletDestroy::Initialize()
{
}

int CBulletDestroy::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)		// 프레임이 끝이면 사라짐
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CBulletDestroy::Late_Update()
{
	Update_Rect();
}

void CBulletDestroy::Render(HDC _DC)
{
	HDC hMemDC;

	// 총알 타입에 따라 출력되는 이미지 다름
	switch (m_eBulletType)
	{
	case BULLETTYPE::BULLET_1:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet_1_Destroy");
		break;
	case BULLETTYPE::BULLET_2:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet_2_Destroy");
		break;
	case BULLETTYPE::BULLET_3:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet_3_Destroy");
		break;
	case BULLETTYPE::BULLET_4:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet_4_Destroy");
		break;
	default:
		return;
	}

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CBulletDestroy::Release()
{
}

// 총알 종류
void CBulletDestroy::Set_BulletType(BULLETTYPE _eBullet)
{
	switch (_eBullet)
	{
	case BULLETTYPE::BULLET_1:
		m_tInfo.iCX = BULLET_1_DESTROY_WIDTH * 3;
		m_tInfo.iCY = BULLET_1_DESTROY_HEIGHT * 3;
		m_iImageWidth = BULLET_1_DESTROY_WIDTH;
		m_iImageHeight = BULLET_1_DESTROY_HEIGHT;

		m_tFrame.iFrameCnt = 0;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 4;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 50;
		break;
	case BULLETTYPE::BULLET_2:
		m_tInfo.iCX = BULLET_2_DESTROY_WIDTH * 3;
		m_tInfo.iCY = BULLET_2_DESTROY_HEIGHT * 3;
		m_iImageWidth = BULLET_2_DESTROY_WIDTH;
		m_iImageHeight = BULLET_2_DESTROY_HEIGHT;

		m_tFrame.iFrameCnt = 0;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 50;
		break;
	case BULLETTYPE::BULLET_3:
		m_tInfo.iCX = BULLET_3_DESTROY_WIDTH * 3;
		m_tInfo.iCY = BULLET_3_DESTROY_HEIGHT * 3;
		m_iImageWidth = BULLET_3_DESTROY_WIDTH;
		m_iImageHeight = BULLET_3_DESTROY_HEIGHT;

		m_tFrame.iFrameCnt = 0;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 7;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 50;
		break;
	case BULLETTYPE::BULLET_4:
		m_tInfo.iCX = BULLET_4_DESTROY_WIDTH * 3;
		m_tInfo.iCY = BULLET_4_DESTROY_HEIGHT * 3;
		m_iImageWidth = BULLET_4_DESTROY_WIDTH;
		m_iImageHeight = BULLET_4_DESTROY_HEIGHT;

		m_tFrame.iFrameCnt = 0;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 50;
		break;
	default:
		throw;
	}

	m_eBulletType = _eBullet;
}
