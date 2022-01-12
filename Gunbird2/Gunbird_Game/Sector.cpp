// 기명준
// NPC 이동을 위한 보이지 않는 사각 영역
// 이동 좌표를 보관

#include "stdafx.h"
#include "Sector.h"


CSector::CSector() : m_fDestPosX(0.f), m_fDestPosY(0.f)
{
}


CSector::~CSector()
{
	Release();
}

void CSector::Initialize()
{
}

// 업데이트
int CSector::Update()
{
	// 삭제
	if (m_bDead)
		return OBJ_DEAD;

	// 배경 스크롤이 정지 상태가 아니면
	if (!g_bStopMapScroll)
		m_tInfo.fY += g_fBackgroundSpeed * 3.f;			// 배경에 맞춰서 Y축 이동

	// 위치 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CSector::Late_Update()
{
	// 맵 하단 밖으로 나가면 삭제
	if (WINCY <= m_tRect.top)
		m_bDead = true;
}

// 렌더
void CSector::Render(HDC _DC)
{
	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CSector::Release()
{
}
