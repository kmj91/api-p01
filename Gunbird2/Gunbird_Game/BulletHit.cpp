// 기명준
// 플레이어 총알이 명중하면 생성되는 이펙트
// 좌우 반전된 두 종류의 이미지를 사용 함

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
	// 이미지 크기 초기화
	m_tInfo.iCX = BULLET_HIT_WIDTH * 3;
	m_tInfo.iCY = BULLET_HIT_HEIGHT * 3;
	m_iImageWidth = BULLET_HIT_WIDTH;
	m_iImageHeight = BULLET_HIT_HEIGHT;
	// 이미지 프레임 초기화
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 17;
	m_tFrame.iFrameScene = rand() % 2;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 10;
}

// 업데이트
int CBulletHit::Update()
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
void CBulletHit::Late_Update()
{
	// 이 오브젝트가 Update에서 생성되서 Update를 건너 뛰고 Late_Update를 하는 경우가 발생함
	// 그래서 Late_Udpate에서 갱신함
	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
}

// 렌더
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
