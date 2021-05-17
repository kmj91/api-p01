#include "stdafx.h"
#include "RobotShark.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_03.h"
#include "Explosion_01.h"
#include "Bullet_3.h"
#include "Bullet_1.h"
#include "Bullet_4.h"

#include "Bomb.h"


CRobotShark::CRobotShark()
	: m_dwOnTime(0), m_dwOutTime(0), m_fDestPosX(0.f), m_fDestPosY(0.f), m_fOutPosX(0.f), m_fOutPosY(0.f),
	m_dwShotTime(0), m_dwShotDelay(0), m_eAttackState(CRobotShark::END),
	m_tBodyFrame{}, m_tPersonFrame{}, m_bOnObject(false), m_bOutObject(false)
{
}


CRobotShark::~CRobotShark()
{
	Release();
}

void CRobotShark::Initialize()
{
	m_tInfo.iCX = ROBOT_SHARK_WIDTH * 3;
	m_tInfo.iCY = ROBOT_SHARK_HEIGHT * 3;
	m_tHitRectPos = { 5 * 3, 2 * 3, 69 * 3, 62 * 3 };
	m_iImageWidth = ROBOT_SHARK_WIDTH;
	m_iImageHeight = ROBOT_SHARK_HEIGHT;

	// 추진기
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 2;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 40;

	// 몸
	m_tBodyFrame.iFrameCnt = 0;
	m_tBodyFrame.iFrameStart = 0;
	m_tBodyFrame.iFrameEnd = 1;
	m_tBodyFrame.iFrameScene = 0;
	m_tBodyFrame.dwFrameTime = GetTickCount();
	m_tBodyFrame.dwFrameSpeed = 100;

	// 탑승자
	m_tPersonFrame.iFrameCnt = 0;
	m_tPersonFrame.iFrameStart = 0;
	m_tPersonFrame.iFrameEnd = 0;
	m_tPersonFrame.iFrameScene = 1;
	m_tPersonFrame.dwFrameTime = GetTickCount();
	m_tPersonFrame.dwFrameSpeed = 999999;

	m_fSpeed = 5.0f;
	m_fHp = 500;

	m_dwShotDelay = 1200;
	m_eAttackState = CRobotShark::SHOT_1;

	// 점수
	m_iScore = 2000;
}

int CRobotShark::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		int irand = rand() % 3 + 1;
		pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// 폭탄 아이템 드랍
		pObj = CAbstractFactory<CBomb>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);

		return OBJ_DEAD;
	}

	if (!m_bOnObject)
	{
		if (Move(m_fDestPosX, m_fDestPosY))
		{
			// 목적지에 도착
			m_bOnObject = true;
			m_dwOnTime = GetTickCount();
			m_fSpeed = 1.f;
		}
	}
	else
	{
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			switch (m_eAttackState)
			{
			case CRobotShark::SHOT_1:
				m_eAttackState = CRobotShark::SHOT_2;
				Shot_1();
				// 스프라이트 프레임
				m_tPersonFrame.iFrameStart = 1;
				m_tPersonFrame.iFrameEnd = 2;
				m_tPersonFrame.dwFrameSpeed = 1000;
				break;
			case CRobotShark::SHOT_2:
				m_eAttackState = CRobotShark::SHOT_3;
				Shot_2();
				// 스프라이트 프레임
				m_tPersonFrame.iFrameStart = 3;
				m_tPersonFrame.iFrameEnd = 4;
				m_tPersonFrame.dwFrameSpeed = 300;
				break;
			case CRobotShark::SHOT_3:
				m_eAttackState = CRobotShark::SHOT_4;
				m_dwShotDelay = 100;
				Shot_3();
				break;
			case CRobotShark::SHOT_4:
				m_eAttackState = CRobotShark::SHOT_1;
				m_dwShotDelay = 2000;
				Shot_3();
				break;
			}
			
			m_dwShotTime = GetTickCount();
		}

		if (m_bOutObject)
		{
			MoveAngle(m_fOutPosX, m_fOutPosY);
		}
		else
		{
			// 퇴장 시간인가?
			DWORD dwTime = GetTickCount();
			if ((dwTime - m_dwOnTime) > m_dwOutTime)
			{
				m_bOutObject = true;
				m_eAttackState = CRobotShark::END;
				m_fAngle = -60.f;
			}
			else
			{
				if (Move(m_fDestPosX - 300.f, m_fDestPosY))
				{
					m_fSpeed = 3.f;
				}
			}
		}
	}

	Update_Rect();
	Frame_Move();
	Frame_MoveEX();

	return OBJ_NOEVENT;
}

void CRobotShark::Late_Update()
{
	if (m_bHpLock)
		if (0 <= m_tRect.top && 0 <= m_tRect.left && WINCX >= m_tRect.right)
			m_bHpLock = false;

	if (!m_bOnObject)
		return;

	if (0 >= m_tRect.bottom)
		m_bRemove = true;
}

void CRobotShark::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RobotShark");

	// 추진기 이펙트
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 몸체
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tBodyFrame.iFrameCnt * m_iImageWidth, m_tBodyFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 탑승자
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tPersonFrame.iFrameCnt * m_iImageWidth, m_tPersonFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 충돌 박스
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CRobotShark::Release()
{
}

bool CRobotShark::Move(float _fDestX, float _fDestY)
{
	float fvx;
	float fvy;
	float fX = _fDestX - m_tInfo.fX;
	float fY = _fDestY - m_tInfo.fY;
	float fDia = sqrtf(fX * fX + fY * fY);	// 현제 위치에서 목적지까지 거리

	if (fDia) {
		fvx = (_fDestX - m_tInfo.fX) / fDia * m_fSpeed;
		fvy = (_fDestY - m_tInfo.fY) / fDia * m_fSpeed;
	}

	// 목적지에 도착하지 않았으면 이동 처리
	if (fDia >= m_fSpeed)
	{
		m_tInfo.fX += fvx;
		m_tInfo.fY += fvy;
		return false;
	}

	return true;
}

bool CRobotShark::MoveAngle(float _fDestX, float _fDestY)
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
		fvx = (_fDestX - m_tInfo.fX) / fDia * m_fSpeed;
		fvy = (_fDestY - m_tInfo.fY) / fDia * m_fSpeed;
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
		return false;
	}

	return true;
}

void CRobotShark::Frame_MoveEX()
{
	if (m_tBodyFrame.dwFrameTime + m_tBodyFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tBodyFrame.iFrameCnt;
		if (m_tBodyFrame.iFrameCnt > m_tBodyFrame.iFrameEnd)
			m_tBodyFrame.iFrameCnt = m_tBodyFrame.iFrameStart;

		m_tBodyFrame.dwFrameTime = GetTickCount();
	}

	if (m_tPersonFrame.dwFrameTime + m_tPersonFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tPersonFrame.iFrameCnt;
		if (m_tPersonFrame.iFrameCnt > m_tPersonFrame.iFrameEnd)
			m_tPersonFrame.iFrameCnt = m_tBodyFrame.iFrameStart;

		m_tPersonFrame.dwFrameTime = GetTickCount();
	}
}

void CRobotShark::Shot_1()
{
	float fCnt = 0;

	while (fCnt < 180.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_3>::Create(m_tInfo.fX - 30.f, m_tInfo.fY - 114.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}

	fCnt = -180.f;
	while (fCnt < 0.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_3>::Create(m_tInfo.fX - 30.f, m_tInfo.fY - 114.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}

	while (fCnt < 180.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_3>::Create(m_tInfo.fX + 33.f, m_tInfo.fY - 114.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}

	fCnt = -180.f;
	while (fCnt < 0.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_3>::Create(m_tInfo.fX + 33.f, m_tInfo.fY - 114.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}
}

void CRobotShark::Shot_2()
{
	// 왼팔
	CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - 87.f, m_tInfo.fY + 60.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - 99.f, m_tInfo.fY + 48.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	// 오른팔
	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + 87.f, m_tInfo.fY + 60.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + 99.f, m_tInfo.fY + 48.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

void CRobotShark::Shot_3()
{
	// 왼팔
	CObj* pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 99.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-130.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 99.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-120.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 69.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-50.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 69.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-60.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	// 오른팔
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 99.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-130.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 99.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-120.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 69.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-50.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 69.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-60.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

