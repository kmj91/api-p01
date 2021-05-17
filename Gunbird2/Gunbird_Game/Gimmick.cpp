#include "stdafx.h"
#include "Gimmick.h"

#include "BmpMgr.h"
#include "ObjMgr.h"


CGimmick::CGimmick() : m_ePreState(CGimmick::END), m_eCurState(CGimmick::END)
{
}


CGimmick::~CGimmick()
{
	Release();
}

void CGimmick::Initialize()
{
	m_tInfo.iCX = GIMMICK_WIDTH * 3;
	m_tInfo.iCY = GIMMICK_HEIGHT * 3;
	m_iImageWidth = GIMMICK_WIDTH;
	m_iImageHeight = GIMMICK_HEIGHT;

	m_fSpeed = 1.5f;

	m_eCurState = CGimmick::IDLE;
}

int CGimmick::Update()
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	if (!g_bStopMapScroll)
	{
		m_tInfo.fY += g_fBackgroundSpeed * 3.f;		// 배경에 맞춰서 Y축 이동
		m_fDestPosY += g_fBackgroundSpeed * 3.f;		// 목적지도 배경 속도에 맞춰서 Y축 이동해야됨
	}
	
	Move();


	Update_Rect();
	Scene_Change();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CGimmick::Late_Update()
{
}

void CGimmick::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Gimmick_Move");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CGimmick::Release()
{
}

void CGimmick::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CGimmick::MOVE_U:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::MOVE_RU:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::MOVE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::MOVE_RD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::MOVE_D:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::MOVE_LD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 5;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::MOVE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 6;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::MOVE_LU:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 7;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::IDLE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 8;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::IDLE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 9;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::IDLE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 10;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CGimmick::IDLE_2:
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

void CGimmick::Move()
{
	float	fX = m_fDestPosX - m_tInfo.fX;
	float	fY = m_fDestPosY - m_tInfo.fY;
	float	fDia = sqrtf(fX * fX + fY * fY);

	// 목적지에 도착하지 않았으면 이동 처리
	if (fDia >= m_fSpeed)
	{
		float fRad = acosf(fX / fDia);

		float fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < m_fDestPosY)
			fAngle *= -1.f;

		m_tInfo.fX += cosf(fAngle * PI / 180.f) * m_fSpeed;
		m_tInfo.fY -= sinf(fAngle * PI / 180.f) * m_fSpeed;

		if ((int)fAngle == 90)
		{
			m_eCurState = CGimmick::MOVE_U;
		}
		else if (0 < (int)fAngle && (int)fAngle < 90)
		{
			m_eCurState = CGimmick::MOVE_RU;
		}
		else if ((int)fAngle == 0)
		{
			m_eCurState = CGimmick::MOVE_R;
		}
		else if (-90 < (int)fAngle && (int)fAngle < 0)
		{
			m_eCurState = CGimmick::MOVE_RD;
		}
		else if ((int)fAngle == -90)
		{
			m_eCurState = CGimmick::MOVE_D;
		}
		else if (-180 < (int)fAngle && (int)fAngle < -90)
		{
			m_eCurState = CGimmick::MOVE_LD;
		}
		else if ((int)fAngle == 180)
		{
			m_eCurState = CGimmick::MOVE_L;
		}
		else if (90 < (int)fAngle && (int)fAngle < 180)
		{
			m_eCurState = CGimmick::MOVE_LU;
		}
	}
	else
	{
		m_eCurState = CGimmick::IDLE;
	}
}
