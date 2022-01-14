// 기명준
// 적 총알이 명중하거나 폭탄에 의해 제거되면 생성되는 이펙트
// 총알 종류에따라 다른 이펙트 출력

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

// 업데이트
int CBulletDestroy::Update()
{
	// 삭제
	if (m_bDead)
		return OBJ_DEAD;

	// 이미지 프레임 이동
	Frame_Move();

	// 프레임이 끝이면 삭제
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_bDead = true;

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CBulletDestroy::Late_Update()
{
	// 이 오브젝트가 Update에서 생성되서 Update를 건너 뛰고 Late_Update를 하는 경우가 발생함
	// 그래서 Late_Udpate에서 갱신함
	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
}

// 렌더
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

// 총알 종류에 따라 이미지 프레임 초기화
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
