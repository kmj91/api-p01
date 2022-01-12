// 기명준
// NPC 샤크 배경 취급
// 게임 플레이 중간에 모습을 보임

#include "stdafx.h"
#include "Shark.h"

#include "BmpMgr.h"
#include "ObjMgr.h"


CShark::CShark() : m_ePreState(CShark::END), m_eCurState(CShark::END)
{
}


CShark::~CShark()
{
	Release();
}

void CShark::Initialize()
{
	// 이미지 크기 설정
	m_tInfo.iCX = SHARK_WIDTH * 3;
	m_tInfo.iCY = SHARK_HEIGHT * 3;
	m_iImageWidth = SHARK_WIDTH;
	m_iImageHeight = SHARK_HEIGHT;
	// 이동 속도
	m_fSpeed = 1.5f;
	// 기본 상태
	m_eCurState = CShark::IDLE;
}

// 업데이트
int CShark::Update()
{
	// 죽음
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
void CShark::Late_Update()
{
}

// 렌더
void CShark::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shark_Move");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CShark::Release()
{
}

// 프레임 씬 변경 처리
void CShark::Scene_Change()
{
	// 이전 상태와 다르면 교체
	if (m_ePreState != m_eCurState)
	{
		// 이동 방향에 따른 이미지 프레임 교체
		switch (m_eCurState)
		{
		case CShark::MOVE_U:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::MOVE_RU:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::MOVE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::MOVE_RD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::MOVE_D:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::MOVE_LD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 5;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::MOVE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 6;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::MOVE_LU:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 7;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::IDLE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 8;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::IDLE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 9;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::IDLE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 10;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CShark::IDLE_2:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 11;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// 이동
void CShark::Move()
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
		m_eCurState = CShark::IDLE;
		return;
	}
	// 좌표 이동
	m_tInfo.fX += fX / fDia * fSpeed;
	m_tInfo.fY += fY / fDia * fSpeed;
	// arccos(cos) = 라디안 값을 구함
	float fRad = acosf(fX / fDia);
	// 라디안 값으로 호도각을 구함
	float fAngle = fRad * 180.f / PI;
	// 목적지의 Y 좌표가 나의 Y 좌표 보다 크면 부호 반전
	if (m_tInfo.fY < m_fDestPosY)
		fAngle *= -1.f;

	// 각도에 따라 이동 상태 교체
	if ((int)fAngle == 90)
	{
		m_eCurState = CShark::MOVE_U;
	}
	else if (0 < (int)fAngle && (int)fAngle < 90)
	{
		m_eCurState = CShark::MOVE_RU;
	}
	else if ((int)fAngle == 0)
	{
		m_eCurState = CShark::MOVE_R;
	}
	else if (-90 < (int)fAngle && (int)fAngle < 0)
	{
		m_eCurState = CShark::MOVE_RD;
	}
	else if ((int)fAngle == -90)
	{
		m_eCurState = CShark::MOVE_D;
	}
	else if (-180 < (int)fAngle && (int)fAngle < -90)
	{
		m_eCurState = CShark::MOVE_LD;
	}
	else if ((int)fAngle == 180)
	{
		m_eCurState = CShark::MOVE_L;
	}
	else if (90 < (int)fAngle && (int)fAngle < 180)
	{
		m_eCurState = CShark::MOVE_LU;
	}
}
