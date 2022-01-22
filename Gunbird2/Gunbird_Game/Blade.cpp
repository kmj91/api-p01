// 기명준
// NPC 블레이드 배경 취급
// 게임 플레이 중간에 모습을 보임

#include "stdafx.h"
#include "Blade.h"

#include "BmpMgr.h"
#include "ObjMgr.h"


CBlade::CBlade() : m_ePreState(CBlade::END), m_eCurState(CBlade::END)
{
}


CBlade::~CBlade()
{
	Release();
}

void CBlade::Initialize()
{
	// 이미지 크기 설정
	m_tInfo.iCX = BLADE_WIDTH * 3;
	m_tInfo.iCY = BLADE_HEIGHT * 3;
	m_iImageWidth = BLADE_WIDTH;
	m_iImageHeight = BLADE_HEIGHT;
	// 이동 속도
	m_fSpeed = 1.5f;
	// 기본 상태
	m_eCurState = CBlade::IDLE;
}

// 업데이트
int CBlade::Update()
{
	// 삭제
	if (m_bDead)
		return OBJ_DEAD;

	// 배경 스크롤이 정지 상태가 아니면
	if (!g_bStopMapScroll) {
		// 배경에 맞춰서 Y축 이동
		m_tInfo.fY += g_fBackgroundSpeed * 3.f;
		// 목적지도 배경 속도에 맞춰서 Y축 이동해야됨
		m_fDestPosY += g_fBackgroundSpeed * 3.f;
	}

	// 이동
	Move();

	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
	// 프레임 씬 변경 처리
	Scene_Change();
	// 이미지 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CBlade::Late_Update()
{
}

// 렌더
void CBlade::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Blade_Move");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CBlade::Release()
{
}

// 프레임 씬 변경 처리
void CBlade::Scene_Change()
{
	// 이전 상태와 다르면 교체
	if (m_ePreState != m_eCurState)
	{
		// 이동 방향에 따른 이미지 프레임 교체
		switch (m_eCurState)
		{
		case CBlade::MOVE_U:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_RU:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_RD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_D:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_LD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 5;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 6;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_LU:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 7;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::IDLE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 8;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::IDLE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 9;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::IDLE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 10;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::IDLE_2:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 11;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// 이동
void CBlade::Move()
{
	float fX = m_fDestPosX - m_tInfo.fX;	// 방향 좌표 X
	float fY = m_fDestPosY - m_tInfo.fY;	// 방향 좌표 Y
	float fDia = sqrtf(fX * fX + fY * fY);	// 현재 위치에서 목적지까지 거리
	float fSpeed = m_fSpeed;				// 이동 속도

	// 목적지까지의 거리가 이동 속도보다 작다면
	// 목표 거리만큼 이동할 수 있게 속도 값을 바꿈
	if (fDia < fSpeed)
		fSpeed = fDia;
	// 목적지에 도착했는지?
	if (fDia - fSpeed == 0) {
		// 대기 상태
		m_eCurState = CBlade::IDLE;
		return;
	}
	// 좌표 이동
	m_tInfo.fX += fX / fDia * fSpeed;
	m_tInfo.fY += fY / fDia * fSpeed;
	// arccos(cos) = 라디안 값을 구함
	float fRad = acosf(fX / fDia);
	// 라디안 값으로 각도를 구함
	float fAngle = fRad * 180.f / PI;
	// 목적지의 Y 좌표가 나의 Y 좌표 보다 크면 부호 반전
	if (m_tInfo.fY < m_fDestPosY)
		fAngle *= -1.f;

	// 각도에 따라 이동 상태 교체
	if ((int)fAngle == 90)
	{
		m_eCurState = CBlade::MOVE_U;
	}
	else if (0 < (int)fAngle && (int)fAngle < 90)
	{
		m_eCurState = CBlade::MOVE_RU;
	}
	else if ((int)fAngle == 0)
	{
		m_eCurState = CBlade::MOVE_R;
	}
	else if (-90 < (int)fAngle && (int)fAngle < 0)
	{
		m_eCurState = CBlade::MOVE_RD;
	}
	else if ((int)fAngle == -90)
	{
		m_eCurState = CBlade::MOVE_D;
	}
	else if (-180 < (int)fAngle && (int)fAngle < -90)
	{
		m_eCurState = CBlade::MOVE_LD;
	}
	else if ((int)fAngle == 180)
	{
		m_eCurState = CBlade::MOVE_L;
	}
	else if (90 < (int)fAngle && (int)fAngle < 180)
	{
		m_eCurState = CBlade::MOVE_LU;
	}
}
