#include "stdafx.h"
#include "MarionBombEffect.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "MarionEffect.h"


CMarionBombEffect::CMarionBombEffect()
	: m_fDestX(0.f), m_fDestY(0.f), m_dwEffectCreateTime(0), m_dwEffectCreateDelay(100)
{
}


CMarionBombEffect::~CMarionBombEffect()
{
	Release();
}

void CMarionBombEffect::Initialize()
{
	m_tInfo.iCX = MARION_GUIDED_BULLET_WIDTH * 3;
	m_tInfo.iCY = MARION_GUIDED_BULLET_HEIGHT * 3;
	m_iImageWidth = MARION_GUIDED_BULLET_WIDTH;
	m_iImageHeight = MARION_GUIDED_BULLET_HEIGHT;

	m_fSpeed = 10.f;
}

int CMarionBombEffect::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_dwEffectTime + m_dwEffectDelay > GetTickCount())		// 이펙트 딜레이
		return OBJ_NOEVENT;
	else
	{
		if (!m_bOnEffect)
		{
			m_bOnEffect = true;
		}
	}

	// 회전 이동
	Move();
	// 각도에 따른 이미지 변경
	Angle();

	return OBJ_NOEVENT;
}

void CMarionBombEffect::Late_Update()
{
	Update_Rect();

	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

void CMarionBombEffect::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Guided_Bullet");

	GdiTransparentBlt(_DC
		, m_tInfo.fX, m_tInfo.fY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CMarionBombEffect::Release()
{
}

void CMarionBombEffect::Move()
{
	float fvx;
	float fvy;
	float fRad;
	float fvx2;
	float fvy2;
	float fX = m_fDestX - m_tInfo.fX;
	float fY = m_fDestY - m_tInfo.fY;
	float fDia = sqrtf(fX * fX + fY * fY);	// 현제 위치에서 목적지까지 거리

	if (fDia) {
		fvx = fX / fDia * m_fSpeed;
		fvy = fY / fDia * m_fSpeed;
	}

	// 목적지에 도착하지 않았으면 이동 처리
	if (fDia >= m_fSpeed)
	{
		// 선회 각도
		fRad = PI / 180.f * m_fAngle;

		fvx2 = cos(fRad) * fvx - sin(fRad) * fvy;
		fvy2 = sin(fRad) * fvx + cos(fRad) * fvy;

		m_tInfo.fX += fvx2;
		m_tInfo.fY += fvy2;

		// 이펙트 생성
		if (m_dwEffectCreateTime + m_dwEffectCreateDelay < GetTickCount())
		{
			m_dwEffectCreateTime = GetTickCount();
			CObj* pObj = CAbstractFactory<CMarionEffect>::Create(m_tInfo.fX, m_tRect.bottom);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		
	}
}

void CMarionBombEffect::Angle()
{
	float	fX = m_fDestX - m_tInfo.fX;
	float	fY = m_fDestY - m_tInfo.fY;
	float	fDia = sqrtf(fX * fX + fY * fY);
	float	fRad = acosf(fX / fDia);

	float fAngle = fRad * 180.f / PI;

	if (m_tInfo.fY < m_fDestY)
		fAngle *= -1.f;

	// 보정
	if (180 < fAngle) {
		fAngle = fAngle - 360;
	}
	else if (fAngle < -180) {
		fAngle = fAngle + 360;
	}

	if (80.f < fAngle && fAngle < 100.f)
		m_iDrawID = CMarionBombEffect::A90;
	else if (60.f <= fAngle && fAngle <= 80.f)
		m_iDrawID = CMarionBombEffect::A70;
	else if (30.f <= fAngle && fAngle < 60.f)
		m_iDrawID = CMarionBombEffect::A45;
	else if (10.f <= fAngle && fAngle < 30.f)
		m_iDrawID = CMarionBombEffect::A20;
	else if (-10.f < fAngle && fAngle < 10.f)
		m_iDrawID = CMarionBombEffect::A0;
	else if (-30.f <= fAngle && fAngle <= -10.f)
		m_iDrawID = CMarionBombEffect::A_20;
	else if (-60.f <= fAngle && fAngle < -30.f)
		m_iDrawID = CMarionBombEffect::A_45;
	else if (-80.f <= fAngle && fAngle < -60.f)
		m_iDrawID = CMarionBombEffect::A_70;
	else if (-100.f < fAngle && fAngle < -80.f)
		m_iDrawID = CMarionBombEffect::A_90;
	else if (-120.f <= fAngle && fAngle <= -100.f)
		m_iDrawID = CMarionBombEffect::A_110;
	else if (-150.f <= fAngle && fAngle < -120.f)
		m_iDrawID = CMarionBombEffect::A_135;
	else if (-170.f <= fAngle && fAngle < -150.f)
		m_iDrawID = CMarionBombEffect::A_160;
	else if (170.f < fAngle && fAngle < -170.f)
		m_iDrawID = CMarionBombEffect::A180;
	else if (150.f <= fAngle && fAngle <= 170.f)
		m_iDrawID = CMarionBombEffect::A160;
	else if (120.f <= fAngle && fAngle < 150.f)
		m_iDrawID = CMarionBombEffect::A135;
	else if (100.f <= fAngle && fAngle < 120.f)
		m_iDrawID = CMarionBombEffect::A110;
}
