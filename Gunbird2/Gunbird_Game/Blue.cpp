#include "stdafx.h"
#include "Blue.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_01.h"
#include "Bullet_1.h"


CBlue::CBlue()
	: m_ePreState(CBlue::END), m_eCurState(CBlue::END), m_dwOnTime(0), m_dwOutTime(0),
	m_fDestPosX(0.f), m_fDestPosY(0.f), m_fOutPosX(0.f), m_fOutPosY(0.f), m_fDestAngle(0.f), m_fOutAngle(0.f),
	m_iShot(0), m_bOnObject(false), m_bOutObject(false)
{
}


CBlue::~CBlue()
{
	Release();
}

void CBlue::Initialize()
{
	m_tInfo.iCX = BLUE_WIDTH * 3;
	m_tInfo.iCY = BLUE_HEIGHT * 3;
	m_tHitRectPos = { 10 * 3, 11 * 3, 32 * 3, 31 * 3 };
	m_iImageWidth = BLUE_WIDTH;
	m_iImageHeight = BLUE_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 40;

	m_fSpeed = 7.0f;

	m_eCurState = CBlue::FRONT;

	// 점수
	m_iScore = 200;
}

int CBlue::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}

	if (!m_bOnObject)
	{
		Move(m_fDestPosX, m_fDestPosY, m_fDestAngle);
	}
	else
	{
		if (m_bOutObject)
		{
			Move(m_fOutPosX, m_fOutPosY, m_fOutAngle);
		}
		else
		{
			// 퇴장 시간인가?
			DWORD dwTime = GetTickCount();
			if ((dwTime - m_dwOnTime) > m_dwOutTime) {
				m_bOutObject = true;
				m_iShot = 1;
			}
		}
	}

	if (m_iShot > 0)
	{
		--m_iShot;
		Shot();
	}

	Update_Rect();
	Scene_Change();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CBlue::Late_Update()
{
	if (m_bHpLock)
	{
		if (0 <= m_tRect.bottom && 0 <= m_tRect.right && WINCX >= m_tRect.left)
		{
			m_bHpLock = false;
			m_iShot = 1;
		}
	}

	if (!m_bOnObject)
		return;
	
	if (0 >= m_tRect.bottom)
		m_bRemove = true;
}

void CBlue::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Blue");

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
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CBlue::Release()
{
}

void CBlue::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBlue::FRONT:
			m_tFrame.iFrameScene = CBlue::FRONT;
			break;
		case CBlue::LEFT_1:
			m_tFrame.iFrameScene = CBlue::LEFT_1;
			break;
		case CBlue::LEFT_2:
			m_tFrame.iFrameScene = CBlue::LEFT_2;
			break;
		case CBlue::LEFT_3:
			m_tFrame.iFrameScene = CBlue::LEFT_3;
			break;
		case CBlue::RIGHT_1:
			m_tFrame.iFrameScene = CBlue::RIGHT_1;
			break;
		case CBlue::RIGHT_2:
			m_tFrame.iFrameScene = CBlue::RIGHT_2;
			break;
		case CBlue::RIGHT_3:
			m_tFrame.iFrameScene = CBlue::RIGHT_3;
			break;
		case CBlue::END:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// 파라미터 좌표로 이동
void CBlue::Move(float _fDestX, float _fDestY, float _fAngle)
{
	float fvx;
	float fvy;
	float fRad;
	float fvx2;
	float fvy2;
	float fX = _fDestX - m_tInfo.fX;
	float fY = _fDestY - m_tInfo.fY;
	float fDia = sqrtf(fX * fX + fY * fY);	// 현제 위치에서 목적지까지 거리

	if (fDia) {
		fvx = fX / fDia * m_fSpeed;
		fvy = fY / fDia * m_fSpeed;
	}

	if (abs(_fAngle) > 30)		// 각도가 너무 높으면
	{
		if (fDia < 30)		// 원점 거리에 도달하기 직전에 너무 빙글빙글 돔
		{
			_fAngle = _fAngle / 2.f;		// 보정해줌
		}
	}

	// 목적지에 도착하지 않았으면 이동 처리
	if (fDia >= m_fSpeed)
	{
		// 선회 각도
		fRad = PI / 180.f * _fAngle;

		fvx2 = cos(fRad) * fvx - sin(fRad) * fvy;
		fvy2 = sin(fRad) * fvx + cos(fRad) * fvy;

		m_tInfo.fX += fvx2;
		m_tInfo.fY += fvy2;

		if (fvx2 > 4.f) {
			m_eCurState = CBlue::RIGHT_3;
		}
		else if (fvx2 > 2.f) {
			m_eCurState = CBlue::RIGHT_2;
		}
		else if (fvx2 > 0.5f) {
			m_eCurState = CBlue::RIGHT_1;
		}
		else if (fvx2 < -4.f) {
			m_eCurState = CBlue::LEFT_3;
		}
		else if (fvx2 < -2.f) {
			m_eCurState = CBlue::LEFT_2;
		}
		else if (fvx2 < -0.5f) {
			m_eCurState = CBlue::LEFT_1;
		}
		else {
			m_eCurState = CBlue::FRONT;
		}
	}
	else
	{
		// 목적지에 도착
		m_eCurState = CBlue::FRONT;
		m_bOnObject = true;
		m_dwOnTime = GetTickCount();
	}
}

void CBlue::Shot()
{
	float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;
	float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;
	float	fDia = sqrtf(fX * fX + fY * fY);
	float	fRad = acosf(fX / fDia);

	float fAngle = fRad * 180.f / PI;

	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		fAngle *= -1.f;

	fAngle = (float)((int)fAngle / 10 * 10);		// 너무 완벽하게 플레이어 위치를 추적하지 못하게 자릿수 버림

	CObj* pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX, m_tInfo.fY - 30.f);
	pObj->Set_Angle(fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}
