#include "stdafx.h"
#include "MarionGuidedBullet.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "BulletHit.h"
#include "MarionEffect.h"


CMarionGuidedBullet::CMarionGuidedBullet()
	: m_dwEffectCreateTime(0), m_dwEffectCreateDelay(100)
	, m_stpSubBulletCnt(nullptr)
{
}


CMarionGuidedBullet::~CMarionGuidedBullet()
{
	Release();
}

void CMarionGuidedBullet::Initialize()
{
	m_tInfo.iCX = MARION_GUIDED_BULLET_WIDTH * 3;
	m_tInfo.iCY = MARION_GUIDED_BULLET_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, MARION_GUIDED_BULLET_WIDTH * 3, MARION_GUIDED_BULLET_HEIGHT * 3 };
	m_iImageWidth = MARION_GUIDED_BULLET_WIDTH;
	m_iImageHeight = MARION_GUIDED_BULLET_HEIGHT;

	m_fSpeed = 16.f;
	m_fDamage = 2.f;
}

int CMarionGuidedBullet::Update()
{
	if (m_bRemove)
	{
		--m_stpSubBulletCnt->count;
		if (m_stpSubBulletCnt->count == 0)		// 화면에서 총알 객체가 모두 사라져야 생성할 수 있음
		{
			m_stpSubBulletCnt->flag = false;
		}
		return OBJ_DEAD;
	}	

	if (m_bDead)
	{
		--m_stpSubBulletCnt->count;
		if (m_stpSubBulletCnt->count == 0)		// 화면에서 총알 객체가 모두 사라져야 생성할 수 있음
		{
			m_stpSubBulletCnt->flag = false;
		}

		CObj* pObj = CAbstractFactory<CBulletHit>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
		

	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	// 이펙트 생성
	if (m_dwEffectCreateTime + m_dwEffectCreateDelay < GetTickCount())
	{
		m_dwEffectCreateTime = GetTickCount();
		CObj* pObj = CAbstractFactory<CMarionEffect>::Create(m_tInfo.fX, m_tRect.bottom);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
	}

	return OBJ_NOEVENT;
}

void CMarionGuidedBullet::Late_Update()
{
	Update_Rect();

	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

void CMarionGuidedBullet::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Guided_Bullet");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 충돌 박스
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CMarionGuidedBullet::Release()
{
}

void CMarionGuidedBullet::Init_Angle()
{
	if (m_pTarget)
	{
		float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;
		float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;
		float	fDia = sqrtf(fX * fX + fY * fY);
		float	fRad = acosf(fX / fDia);

		m_fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			m_fAngle *= -1.f;
	}

	if (80.f < m_fAngle && m_fAngle < 100.f)
		m_iDrawID = CMarionGuidedBullet::A90;
	else if (60.f <= m_fAngle && m_fAngle <= 80.f)
		m_iDrawID = CMarionGuidedBullet::A70;
	else if (30.f <= m_fAngle && m_fAngle < 60.f)
		m_iDrawID = CMarionGuidedBullet::A45;
	else if (10.f <= m_fAngle && m_fAngle < 30.f)
		m_iDrawID = CMarionGuidedBullet::A20;
	else if (-10.f < m_fAngle && m_fAngle < 10.f)
		m_iDrawID = CMarionGuidedBullet::A0;
	else if (-30.f <= m_fAngle && m_fAngle <= -10.f)
		m_iDrawID = CMarionGuidedBullet::A_20;
	else if (-60.f <= m_fAngle && m_fAngle < -30.f)
		m_iDrawID = CMarionGuidedBullet::A_45;
	else if (-80.f <= m_fAngle && m_fAngle < -60.f)
		m_iDrawID = CMarionGuidedBullet::A_70;
	else if (-100.f < m_fAngle && m_fAngle < -80.f)
		m_iDrawID = CMarionGuidedBullet::A_90;
	else if (-120.f <= m_fAngle && m_fAngle <= -100.f)
		m_iDrawID = CMarionGuidedBullet::A_110;
	else if (-150.f <= m_fAngle && m_fAngle < -120.f)
		m_iDrawID = CMarionGuidedBullet::A_135;
	else if (-170.f <= m_fAngle && m_fAngle < -150.f)
		m_iDrawID = CMarionGuidedBullet::A_160;
	else if (170.f < m_fAngle && m_fAngle < -170.f)
		m_iDrawID = CMarionGuidedBullet::A180;
	else if (150.f <= m_fAngle && m_fAngle <= 170.f)
		m_iDrawID = CMarionGuidedBullet::A160;
	else if (120.f <= m_fAngle && m_fAngle < 150.f)
		m_iDrawID = CMarionGuidedBullet::A135;
	else if (100.f <= m_fAngle && m_fAngle < 120.f)
		m_iDrawID = CMarionGuidedBullet::A110;
}
