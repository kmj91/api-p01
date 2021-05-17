#include "stdafx.h"
#include "CactusCannon.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_02_1.h"
#include "Bullet_2.h"
#include "Coin.h"


CCactusCannon::CCactusCannon() : m_ePreState(CCactusCannon::END), m_eCurState(CCactusCannon::END), m_fDestPosY(0.f),
	m_dwShotTime(0), m_dwShotDelay(0), m_bOnObject(false)
{
}


CCactusCannon::~CCactusCannon()
{
	Release();
}

void CCactusCannon::Initialize()
{
	m_tInfo.iCX = CANNON_WIDTH * 3;
	m_tInfo.iCY = CANNON_HEIGHT * 3;
	m_tHitRectPos = { 6 * 3, 1 * 3, 26 * 3, 33 * 3 };
	m_iImageWidth = CANNON_WIDTH;
	m_iImageHeight = CANNON_HEIGHT;

	m_fHp = 60.f;

	m_iDrawID = CCactusCannon::A_90;
	m_eCurState = CCactusCannon::HIDE;

	m_dwShotDelay = 500;

	// 점수
	m_iScore = 500;
}

int CCactusCannon::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead)
	{
		if (m_ePreState != CCactusCannon::DESTROY)
		{
			m_eCurState = CCactusCannon::DESTROY;
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

	if (m_ePreState == CCactusCannon::ON)
	{
		Aim();			// 조준
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			Shot();
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
		}
	}
	else if (m_ePreState == CCactusCannon::HIDE)
	{
		if (m_bOnObject)
		{
			if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
			{
				m_bHpLock = false;
				m_eCurState = CCactusCannon::ON;
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

void CCactusCannon::Late_Update()
{
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

void CCactusCannon::Render(HDC _DC)
{
	HDC hMemDC;

	if (m_ePreState == CCactusCannon::HIDE)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cannon_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		int iY;
		if (m_tFrame.iFrameCnt == 0)
			iY = 13 * 3;
		else if (m_tFrame.iFrameCnt == 1)
			iY = 17 * 3;
		else if (m_tFrame.iFrameCnt == 2)
			iY = 21 * 3;
		else if (m_tFrame.iFrameCnt == 3)
			iY = 25 * 3;
		else if (m_tFrame.iFrameCnt == 4)
			iY = 28 * 3;
		else if (m_tFrame.iFrameCnt == 5)
			iY = 29 * 3;
		else if (m_tFrame.iFrameCnt == 6)
			iY = 29 * 3;

		// 선인장
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cactus");
		GdiTransparentBlt(_DC
			, (int)(m_tInfo.fX - ((CACTUS_WIDTH * 3) >> 1)) - 2, (int)((m_tInfo.fY - iY) - ((CACTUS_HEIGHT * 3) >> 1))
			, CACTUS_WIDTH * 3, CACTUS_HEIGHT * 3
			, hMemDC
			, 0, 0
			, CACTUS_WIDTH, CACTUS_HEIGHT
			, RGB(255, 0, 255));
	}
	else if (m_ePreState == CCactusCannon::ON)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cannon_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_iDrawID * m_iImageWidth, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 선인장
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cactus");
		GdiTransparentBlt(_DC
			, (int)(m_tInfo.fX - ((CACTUS_WIDTH * 3) >> 1)) - 2, (int)((m_tInfo.fY - 29 * 3) - ((CACTUS_HEIGHT * 3) >> 1))
			, CACTUS_WIDTH * 3, CACTUS_HEIGHT * 3
			, hMemDC
			, 0, 0
			, CACTUS_WIDTH, CACTUS_HEIGHT
			, RGB(255, 0, 255));
	}
	else if (m_ePreState == CCactusCannon::DESTROY)
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

void CCactusCannon::Release()
{
}

void CCactusCannon::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CCactusCannon::HIDE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
		case CCactusCannon::ON:
			break;
		case CCactusCannon::DESTROY:
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

void CCactusCannon::Aim()
{
	float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;
	float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;
	float	fDia = sqrtf(fX * fX + fY * fY);
	float	fRad = acosf(fX / fDia);

	m_fAngle = fRad * 180.f / PI;

	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		m_fAngle *= -1.f;

	if (-5.f <= m_fAngle && m_fAngle < 5.f)
		m_iDrawID = CCactusCannon::A0;
	if (-10.f <= m_fAngle && m_fAngle < -5.f)
		m_iDrawID = CCactusCannon::A_5;
	else if (-20.f <= m_fAngle && m_fAngle < -10.f)
		m_iDrawID = CCactusCannon::A_15;
	else if (-30.f <= m_fAngle && m_fAngle < -20.f)
		m_iDrawID = CCactusCannon::A_30;
	else if (-45.f <= m_fAngle && m_fAngle < -30.f)
		m_iDrawID = CCactusCannon::A_45;
	else if (-60.f <= m_fAngle && m_fAngle < -45.f)
		m_iDrawID = CCactusCannon::A_60;
	else if (-75.f <= m_fAngle && m_fAngle < -60.f)
		m_iDrawID = CCactusCannon::A_75;
	else if (-85.f <= m_fAngle && m_fAngle < -75.f)
		m_iDrawID = CCactusCannon::A_85;
	else if (-95.f <= m_fAngle && m_fAngle < -85.f)
		m_iDrawID = CCactusCannon::A_90;
	else if (-105.f <= m_fAngle && m_fAngle < -95.f)
		m_iDrawID = CCactusCannon::A_95;
	else if (-120.f <= m_fAngle && m_fAngle < -105.f)
		m_iDrawID = CCactusCannon::A_105;
	else if (-135.f <= m_fAngle && m_fAngle < -120.f)
		m_iDrawID = CCactusCannon::A_120;
	else if (-150.f <= m_fAngle && m_fAngle < -135.f)
		m_iDrawID = CCactusCannon::A_135;
	else if (-160.f <= m_fAngle && m_fAngle < -150.f)
		m_iDrawID = CCactusCannon::A_150;
	else if (-170.f <= m_fAngle && m_fAngle < -160.f)
		m_iDrawID = CCactusCannon::A_165;
	else if (-175.f <= m_fAngle && m_fAngle < -170.f)
		m_iDrawID = CCactusCannon::A_175;
	else if (-175.f > m_fAngle || 175.f <= m_fAngle)
		m_iDrawID = CCactusCannon::A180;
	else if (170.f <= m_fAngle && m_fAngle < 175.f)
		m_iDrawID = CCactusCannon::A175;
	else if (160.f <= m_fAngle && m_fAngle < 170.f)
		m_iDrawID = CCactusCannon::A165;
	else if (150.f <= m_fAngle && m_fAngle < 160.f)
		m_iDrawID = CCactusCannon::A150;
	else if (135.f <= m_fAngle && m_fAngle < 150.f)
		m_iDrawID = CCactusCannon::A135;
	else if (120.f <= m_fAngle && m_fAngle < 135.f)
		m_iDrawID = CCactusCannon::A120;
	else if (105.f <= m_fAngle && m_fAngle < 120.f)
		m_iDrawID = CCactusCannon::A105;
	else if (95.f <= m_fAngle && m_fAngle < 105.f)
		m_iDrawID = CCactusCannon::A95;
	else if (85.f <= m_fAngle && m_fAngle < 95.f)
		m_iDrawID = CCactusCannon::A90;
	else if (75.f <= m_fAngle && m_fAngle < 85.f)
		m_iDrawID = CCactusCannon::A85;
	else if (60.f <= m_fAngle && m_fAngle < 75.f)
		m_iDrawID = CCactusCannon::A75;
	else if (45.f <= m_fAngle && m_fAngle < 60.f)
		m_iDrawID = CCactusCannon::A60;
	else if (30.f <= m_fAngle && m_fAngle < 45.f)
		m_iDrawID = CCactusCannon::A45;
	else if (20.f <= m_fAngle && m_fAngle < 30.f)
		m_iDrawID = CCactusCannon::A30;
	else if (10.f <= m_fAngle && m_fAngle < 20.f)
		m_iDrawID = CCactusCannon::A15;
	else if (5.f <= m_fAngle && m_fAngle < 10.f)
		m_iDrawID = CCactusCannon::A5;
}

void CCactusCannon::Shot()
{
	CObj* pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX, m_tInfo.fY);
	pObj->Set_Angle(m_fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}
