// 기명준
// 폭탄 횟수 증가 아이템
// 화면 안에서 특정 각도로 부딪히며 돌아다님

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
	// 이미지 크기 및 Hit RECT 초기화
	m_tInfo.iCX = BOMB_WIDTH * 3;
	m_tInfo.iCY = BOMB_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, BOMB_WIDTH * 3, BOMB_HEIGHT * 3 };
	m_iImageWidth = BOMB_WIDTH;
	m_iImageHeight = BOMB_HEIGHT;
	// 이미지 프레임 초기화
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;
	// 이동 속도
	m_fSpeed = 4.0f;
	// 랜덤 각도 초기화
	int iRadValue = rand() % 20;
	m_fAngle = -70.f + (float)iRadValue;
	m_fRadian = PI * m_fAngle / 180.f;
	// 아이템 타입 초기화
	m_enItemType = ITEM::BOMB;
}

// 업데이트
int CBomb::Update()
{
	// 삭제
	if (m_bDead)
		return OBJ_DEAD;

	// 이동
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


	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
	// 이미지 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CBomb::Late_Update()
{
}

// 렌더
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

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CBomb::Release()
{
}
