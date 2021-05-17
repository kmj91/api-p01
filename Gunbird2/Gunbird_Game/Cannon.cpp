#include "stdafx.h"
#include "Cannon.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_02_1.h"
#include "Bullet_2.h"
#include "Coin.h"


CCannon::CCannon() : m_ePreState(CCannon::END), m_eCurState(CCannon::END), m_fDestPosY(0.f),
	m_dwShotTime(0), m_dwShotDelay(0), m_bOnObject(false)
{
}


CCannon::~CCannon()
{
	Release();
}

void CCannon::Initialize()
{
	m_tInfo.iCX = CANNON_WIDTH * 3;
	m_tInfo.iCY = CANNON_HEIGHT * 3;
	m_tHitRectPos = { 6 * 3, 1 * 3, 26 * 3, 33 * 3 };
	m_iImageWidth = CANNON_WIDTH;
	m_iImageHeight = CANNON_HEIGHT;

	m_fHp = 60.f;

	m_iDrawID = CCannon::A_90;
	m_eCurState = CCannon::HIDE;

	m_dwShotDelay = 2000;

	// 점수
	m_iScore = 500;
}

int CCannon::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead)
	{
		if (m_ePreState != CCannon::DESTROY)
		{
			m_eCurState = CCannon::DESTROY;
			m_bHpLock = true;		// 파괴되서 HP 다시 락
			CObj* pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			// 아이템 코인 생성
			pObj = CAbstractFactory<CCoin>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);

			// 충돌 렉트
			m_tHitRectPos = { 0,0,0,0 };
		}
	}

	m_tInfo.fY += g_fBackgroundSpeed * 3.f;			// 배경에 맞춰서 Y축 이동

	if (m_ePreState == CCannon::ON)
	{
		Aim();			// 조준
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			Shot();
			m_dwShotTime = GetTickCount();
		}
	}
	else if(m_ePreState == CCannon::HIDE)
	{
		if (m_bOnObject)
		{
			if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
			{
				m_bHpLock = false;
				m_eCurState = CCannon::ON;
				m_dwShotTime = GetTickCount();
			}	
		}
		else
		{
			if (m_tInfo.fY > m_fDestPosY)
			{
				m_bOnObject = true;						// 숨어있다가 나옴
				m_tFrame.iFrameCnt = 0;
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 6;
				m_tFrame.iFrameScene = 0;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.dwFrameSpeed = 80;
			}
		}
	}

	Update_Rect();
	Scene_Change();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CCannon::Late_Update()
{
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

void CCannon::Render(HDC _DC)
{
	HDC hMemDC;

	if (m_ePreState == CCannon::HIDE)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cannon_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
	}
	else if (m_ePreState == CCannon::ON)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cannon_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_iDrawID * m_iImageWidth, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
	}
	else if (m_ePreState == CCannon::DESTROY)
	{
		// 잔해
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cannon_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
	}

	// 충돌 박스
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CCannon::Release()
{
}

void CCannon::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CCannon::HIDE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
		case CCannon::ON:
			break;
		case CCannon::DESTROY:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 300;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CCannon::Aim()
{
	float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;
	float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;
	float	fDia = sqrtf(fX * fX + fY * fY);
	float	fRad = acosf(fX / fDia);

	m_fAngle = fRad * 180.f / PI;

	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		m_fAngle *= -1.f;

	if (-5.f <= m_fAngle && m_fAngle < 5.f)
		m_iDrawID = CCannon::A0;
	else if (-10.f <= m_fAngle && m_fAngle < -5.f)
		m_iDrawID = CCannon::A_5;
	else if (-20.f <= m_fAngle && m_fAngle < -10.f)
		m_iDrawID = CCannon::A_15;
	else if (-30.f <= m_fAngle && m_fAngle < -20.f)
		m_iDrawID = CCannon::A_30;
	else if (-45.f <= m_fAngle && m_fAngle < -30.f)
		m_iDrawID = CCannon::A_45;
	else if (-60.f <= m_fAngle && m_fAngle < -45.f)
		m_iDrawID = CCannon::A_60;
	else if (-75.f <= m_fAngle && m_fAngle < -60.f)
		m_iDrawID = CCannon::A_75;
	else if (-85.f <= m_fAngle && m_fAngle < -75.f)
		m_iDrawID = CCannon::A_85;
	else if (-95.f <= m_fAngle && m_fAngle < -85.f)
		m_iDrawID = CCannon::A_90;
	else if (-105.f <= m_fAngle && m_fAngle < -95.f)
		m_iDrawID = CCannon::A_95;
	else if (-120.f <= m_fAngle && m_fAngle < -105.f)
		m_iDrawID = CCannon::A_105;
	else if (-135.f <= m_fAngle && m_fAngle < -120.f)
		m_iDrawID = CCannon::A_120;
	else if (-150.f <= m_fAngle && m_fAngle < -135.f)
		m_iDrawID = CCannon::A_135;
	else if (-160.f <= m_fAngle && m_fAngle < -150.f)
		m_iDrawID = CCannon::A_150;
	else if (-170.f <= m_fAngle && m_fAngle < -160.f)
		m_iDrawID = CCannon::A_165;
	else if (-175.f <= m_fAngle && m_fAngle < -170.f)
		m_iDrawID = CCannon::A_175;
	else if (-175.f > m_fAngle || 175.f <= m_fAngle)
		m_iDrawID = CCannon::A180;
	else if (170.f <= m_fAngle && m_fAngle < 175.f)
		m_iDrawID = CCannon::A175;
	else if (160.f <= m_fAngle && m_fAngle < 170.f)
		m_iDrawID = CCannon::A165;
	else if (150.f <= m_fAngle && m_fAngle < 160.f)
		m_iDrawID = CCannon::A150;
	else if (135.f <= m_fAngle && m_fAngle < 150.f)
		m_iDrawID = CCannon::A135;
	else if (120.f <= m_fAngle && m_fAngle < 135.f)
		m_iDrawID = CCannon::A120;
	else if (105.f <= m_fAngle && m_fAngle < 120.f)
		m_iDrawID = CCannon::A105;
	else if (95.f <= m_fAngle && m_fAngle < 105.f)
		m_iDrawID = CCannon::A95;
	else if (85.f <= m_fAngle && m_fAngle < 95.f)
		m_iDrawID = CCannon::A90;
	else if (75.f <= m_fAngle && m_fAngle < 85.f)
		m_iDrawID = CCannon::A85;
	else if (60.f <= m_fAngle && m_fAngle < 75.f)
		m_iDrawID = CCannon::A75;
	else if (45.f <= m_fAngle && m_fAngle < 60.f)
		m_iDrawID = CCannon::A60;
	else if (30.f <= m_fAngle && m_fAngle < 45.f)
		m_iDrawID = CCannon::A45;
	else if (20.f <= m_fAngle && m_fAngle < 30.f)
		m_iDrawID = CCannon::A30;
	else if (10.f <= m_fAngle && m_fAngle < 20.f)
		m_iDrawID = CCannon::A15;
	else if (5.f <= m_fAngle && m_fAngle < 10.f)
		m_iDrawID = CCannon::A5;
}

void CCannon::Shot()
{
	CObj* pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX, m_tInfo.fY);
	pObj->Set_Angle(m_fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}
