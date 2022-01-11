// �����
// ���� ���� �Ķ��� �κ�
// �̵��� �� �ٸ� ������Ʈ ó�� �Ȱ��� �ӵ��� �����̸�
// ������ ����� ��ġ ���� �ӽ����� �ȴ� �� ���� ������ ��
// �������� �̵��� ��ǥ �ӵ� ���� ���� ������

#include "stdafx.h"
#include "RobotBlue.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "Effect.h"
#include "Explosion_03.h"
#include "Explosion_02_1.h"
#include "Explosion_01.h"
#include "Bullet_4.h"
#include "PowerUp.h"


CRobotBlue::CRobotBlue()
	: m_ePreState(CRobotBlue::END), m_eCurState(CRobotBlue::END),
	m_iOldFrame(0), m_iActionCnt(0), m_dwShotTime(0), m_dwShotDelay(0), m_tTopFrame{}, m_bOnObject(false)
{
}


CRobotBlue::~CRobotBlue()
{
	Release();
}

void CRobotBlue::Initialize()
{
	// �⺻ ��ġ �� ����
	m_tInfo.iCX = ROBOT_BLUE_WIDTH * 3;
	m_tInfo.iCY = ROBOT_BLUE_HEIGHT * 3;
	m_tHitRectPos = { 21 * 3, 16 * 3, 85 * 3, 70 * 3 };
	m_iImageWidth = ROBOT_BLUE_WIDTH;
	m_iImageHeight = ROBOT_BLUE_HEIGHT;
	// �⺻ �ִϸ��̼� ������ ����
	m_tTopFrame.iFrameCnt = 0;
	m_tTopFrame.iFrameStart = 0;
	m_tTopFrame.iFrameEnd = 7;
	m_tTopFrame.iFrameScene = 0;
	m_tTopFrame.dwFrameTime = GetTickCount();
	m_tTopFrame.dwFrameSpeed = 100;
	// ü��
	m_fHp = 650.f;
	// �⺻ ����
	m_eCurState = CRobotBlue::IDLE_R;

	// ����
	m_iScore = 1500;
}

// ������Ʈ
int CRobotBlue::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;
	// ����
	if (m_bDead)
	{
		if (m_ePreState != CRobotBlue::DESTROY)
		{
			m_eCurState = CRobotBlue::DESTROY;
			// �ı��Ǽ� HP �ٽ� ��
			m_bHpLock = true;

			// �Ŀ��� ������ ���
			CObj* pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);

			pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			// �浹 ��Ʈ
			m_tHitRectPos = { 0,0,0,0 };
		}
	}

	// ��濡 ���缭 Y�� �̵�
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;

	// AI ����
	if (m_ePreState != CRobotBlue::DESTROY)
	{
		if (m_iActionCnt == 0)
		{
			if (Move_RU())
				++m_iActionCnt;
		}
		else if (m_iActionCnt == 1)
		{
			if (Move_RU())
				++m_iActionCnt;
		}
		else if (m_iActionCnt == 2)
		{
			if (Move_RU())
				++m_iActionCnt;
		}
		else if (m_iActionCnt == 3)
		{
			if (Move_RU())
			{
				++m_iActionCnt;
				m_dwShotTime = GetTickCount();
				m_dwShotDelay = 500;
				m_tTopFrame.dwFrameSpeed = 10;
			}

		}
		else if (m_iActionCnt == 4)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
			{
				++m_iActionCnt;
				Shot_1(0.f);
				m_dwShotTime = GetTickCount();
				m_dwShotDelay = 150;
			}
		}
		else if (m_iActionCnt == 5)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
			{
				++m_iActionCnt;
				Shot_1(10.f);
				m_dwShotTime = GetTickCount();
				m_dwShotDelay = 300;
			}
		}
		else if (m_iActionCnt == 6)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
				m_tTopFrame.dwFrameSpeed = 30;

			if (Move_RU())
			{
				++m_iActionCnt;
				m_dwShotTime = GetTickCount();
			}
		}
		else if (m_iActionCnt == 7)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
				m_tTopFrame.dwFrameSpeed = 50;

			if (Move_RU())
			{
				++m_iActionCnt;
				m_dwShotTime = GetTickCount();
			}
		}
		else if (m_iActionCnt == 8)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
				m_tTopFrame.dwFrameSpeed = 70;

			if (Move_RU())
			{
				++m_iActionCnt;
				m_dwShotTime = GetTickCount();
			}
		}
		else if (m_iActionCnt == 9)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
				m_tTopFrame.dwFrameSpeed = 100;

			if (Move_RU())
			{
				++m_iActionCnt;
				m_dwShotTime = GetTickCount();
				m_dwShotDelay = 500;
				m_tTopFrame.dwFrameSpeed = 10;
			}
		}
		else if (m_iActionCnt == 10)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
			{
				++m_iActionCnt;
				Shot_2();
				m_dwShotTime = GetTickCount();
				m_dwShotDelay = 1000;
			}
		}
		else if (m_iActionCnt == 11)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
			{
				++m_iActionCnt;
				Shot_2();
				m_dwShotTime = GetTickCount();
			}
		}
		else if (m_iActionCnt == 12)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
			{
				++m_iActionCnt;
				Shot_1(0.f);
				m_dwShotTime = GetTickCount();
				m_dwShotDelay = 150;
			}
		}
		else if (m_iActionCnt == 13)
		{
			if (m_dwShotTime + m_dwShotDelay < GetTickCount())
			{
				++m_iActionCnt;
				Shot_1(10.f);
				m_dwShotTime = GetTickCount();
				m_dwShotDelay = 5000;
			}
		}
		//else if (m_iActionCnt == 14)
		//{
		//	if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		//	{
		//		if (Move_RU())
		//			++m_iActionCnt;
		//	}
		//}
		//else if (m_iActionCnt == 15)
		//{
		//	if (Move_RU())
		//		++m_iActionCnt;
		//}
	}

	// ����
	Aim();

	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();
	// ������ �� ���� ó��
	Scene_Change();
	// �̹��� ������ �̵�
	Frame_Move();
	// �Ѳ� �̹��� ������ �̵�
	Frame_MoveEX();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CRobotBlue::Late_Update()
{
	// �ı��� ���°� �ƴϰ� ���� ���¸� ��ġ����
	if (m_ePreState != CRobotBlue::DESTROY && m_bHpLock) {
		// ȭ�� ������ ������
		if (0 <= m_tRect.bottom && 0 <= m_tRect.right && WINCX >= m_tRect.left) {
			// ���� ����
			m_bHpLock = false;
			// ��ġ �÷��� ON
			m_bOnObject = true;
		}
	}

	// �� �ϴ����� ������ ����
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
void CRobotBlue::Render(HDC _DC)
{
	HDC hMemDC;

	// �ı��� ���¿� �Ϲ� ���� ����
	if (m_ePreState == CRobotBlue::DESTROY)
	{
		// ����
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RobotEtc");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
	}
	else
	{
		// �ٸ�
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RobotReg");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// ��ü
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RobotBody_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_iDrawID * m_iImageWidth, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// �Ѳ�
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RobotEtc");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tTopFrame.iFrameCnt * m_iImageWidth, m_tTopFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
	}

	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CRobotBlue::Release()
{
}

// ������ �� ���� ó��
void CRobotBlue::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CRobotBlue::IDLE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			break;
		case CRobotBlue::IDLE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			break;
		case CRobotBlue::MOVE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			break;
		case CRobotBlue::MOVE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			break;
		case CRobotBlue::DESTROY:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// �Ѳ� �̹��� ������ �̵�
void CRobotBlue::Frame_MoveEX()
{
	if (m_tTopFrame.dwFrameTime + m_tTopFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tTopFrame.iFrameCnt;
		if (m_tTopFrame.iFrameCnt > m_tTopFrame.iFrameEnd)
			m_tTopFrame.iFrameCnt = m_tTopFrame.iFrameStart;

		m_tTopFrame.dwFrameTime = GetTickCount();
	}
}

// ������ ����
// _fAngle : ȣ���� ī��Ʈ
void CRobotBlue::Shot_1(float _fAngle)
{
	float fCnt = _fAngle;

	while (fCnt < 180.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY - 69.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 50.f;
	}

	fCnt = -180.f + _fAngle;
	while (fCnt < 0.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY - 69.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 50.f;
	}
}

// ����
void CRobotBlue::Shot_2()
{
	float fAngle = m_fAngle;

	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		fAngle *= -1.f;

	CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY - 69.f);
	pObj->Set_Angle(fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

// ����
void CRobotBlue::Aim()
{
	float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;
	float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;
	float	fDia = sqrtf(fX * fX + fY * fY);
	float	fRad = acosf(fX / fDia);

	m_fAngle = fRad * 180.f / PI;

	if (m_fAngle < 43)
		m_iDrawID = CRobotBlue::A40;
	else if (m_fAngle < 46)
		m_iDrawID = CRobotBlue::A43;
	else if (m_fAngle < 49)
		m_iDrawID = CRobotBlue::A46;
	else if (m_fAngle < 52)
		m_iDrawID = CRobotBlue::A49;
	else if (m_fAngle < 55)
		m_iDrawID = CRobotBlue::A52;
	else if (m_fAngle < 58)
		m_iDrawID = CRobotBlue::A55;
	else if (m_fAngle < 61)
		m_iDrawID = CRobotBlue::A58;
	else if (m_fAngle < 64)
		m_iDrawID = CRobotBlue::A61;
	else if (m_fAngle < 67)
		m_iDrawID = CRobotBlue::A64;
	else if (m_fAngle < 70)
		m_iDrawID = CRobotBlue::A67;
	else if (m_fAngle < 73)
		m_iDrawID = CRobotBlue::A70;
	else if (m_fAngle < 76)
		m_iDrawID = CRobotBlue::A73;
	else if (m_fAngle < 79)
		m_iDrawID = CRobotBlue::A76;
	else if (m_fAngle < 82)
		m_iDrawID = CRobotBlue::A79;
	else if (m_fAngle < 85)
		m_iDrawID = CRobotBlue::A82;
	else if (m_fAngle < 88)
		m_iDrawID = CRobotBlue::A85;
	else if (m_fAngle < 91)
		m_iDrawID = CRobotBlue::A88;
	else if (m_fAngle < 94)
		m_iDrawID = CRobotBlue::A91;
	else if (m_fAngle < 97)
		m_iDrawID = CRobotBlue::A94;
	else if (m_fAngle < 100)
		m_iDrawID = CRobotBlue::A97;
	else if (m_fAngle < 103)
		m_iDrawID = CRobotBlue::A100;
	else if (m_fAngle < 106)
		m_iDrawID = CRobotBlue::A103;
	else if (m_fAngle < 109)
		m_iDrawID = CRobotBlue::A106;
	else if (m_fAngle < 112)
		m_iDrawID = CRobotBlue::A109;
	else if (m_fAngle < 115)
		m_iDrawID = CRobotBlue::A112;
	else if (m_fAngle < 118)
		m_iDrawID = CRobotBlue::A115;
	else if (m_fAngle < 121)
		m_iDrawID = CRobotBlue::A118;
	else if (m_fAngle < 124)
		m_iDrawID = CRobotBlue::A121;
	else if (m_fAngle < 127)
		m_iDrawID = CRobotBlue::A124;
	else if (m_fAngle < 130)
		m_iDrawID = CRobotBlue::A127;
	else if (m_fAngle < 133)
		m_iDrawID = CRobotBlue::A130;
	else if (m_fAngle < 136)
		m_iDrawID = CRobotBlue::A133;
	else if (m_fAngle < 139)
		m_iDrawID = CRobotBlue::A136;
}

// �̵�
// �̵��� �� �ٸ� ������Ʈ ó�� �Ȱ��� �ӵ��� �����̸�
// ������ ����� ��ġ ���� �ӽ����� �ȴ� �� ���� ������ ��
// ������ �������� �̵��� ��ǥ �ӵ� ���� ���� ������
bool CRobotBlue::Move_RU()
{
	if (m_eCurState == CRobotBlue::IDLE_R)
	{
		m_eCurState = CRobotBlue::MOVE_R;
	}
	else if (m_eCurState == CRobotBlue::IDLE_L)
	{
		m_eCurState = CRobotBlue::MOVE_L;
	}


	if (m_eCurState == CRobotBlue::MOVE_R)
	{
		if (m_tFrame.iFrameCnt == 0)
		{
			m_tFrame.dwFrameSpeed = 150;
		}
		else if (m_tFrame.iFrameCnt == 1)
		{
			if (m_iOldFrame != 1)
			{
				m_iOldFrame = 1;
				m_tInfo.fX += 12.f;
				m_tInfo.fY -= 12.f;
				m_tFrame.dwFrameSpeed = 100;

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
				CSoundMgr::Get_Instance()->PlaySound(L"Robot_Move.wav", CSoundMgr::MONSTER);
			}	
		}
		else if (m_tFrame.iFrameCnt == 2)
		{
			if (m_iOldFrame != 2)
			{
				m_iOldFrame = 2;
				m_tInfo.fX += 9.f;
				m_tInfo.fY -= 12.f;
				m_tFrame.dwFrameSpeed = 70;
			}
		}
		else if (m_tFrame.iFrameCnt == 3)
		{
			if (m_iOldFrame != 3)
			{
				m_iOldFrame = 3;
				m_tInfo.fX += 12.f;
				m_tInfo.fY -= 9.f;
				m_tFrame.dwFrameSpeed = 50;
			}
		}
		else if (m_tFrame.iFrameCnt == 4)
		{
			if (m_iOldFrame != 4)
			{
				m_iOldFrame = 4;
				m_tInfo.fX += 12.f;
				m_tInfo.fY -= 9.f;
				m_tFrame.dwFrameSpeed = 50;
			}
		}
		else if (m_tFrame.iFrameCnt == 5)
		{
			if (m_iOldFrame != 5)
			{
				m_iOldFrame = 5;
				m_tInfo.fX += 9.f;
				m_tInfo.fY -= 6.f;
				m_tFrame.dwFrameSpeed = 50;
			}

			m_eCurState = CRobotBlue::IDLE_L;
			return true;
		}
	}
	else if(m_eCurState == CRobotBlue::MOVE_L)
	{
		if (m_tFrame.iFrameCnt == 0)
		{
			m_tFrame.dwFrameSpeed = 150;
		}
		else if (m_tFrame.iFrameCnt == 1)
		{
			if (m_iOldFrame != 1)
			{
				m_iOldFrame = 1;
				m_tInfo.fX += 9.f;
				m_tInfo.fY -= 6.f;
				m_tFrame.dwFrameSpeed = 100;

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
				CSoundMgr::Get_Instance()->PlaySound(L"Robot_Move.wav", CSoundMgr::MONSTER);
			}
		}
		else if (m_tFrame.iFrameCnt == 2)
		{
			if (m_iOldFrame != 2)
			{
				m_iOldFrame = 2;
				m_tInfo.fX += 12.f;
				m_tInfo.fY -= 12.f;
				m_tFrame.dwFrameSpeed = 70;
			}
		}
		else if (m_tFrame.iFrameCnt == 3)
		{
			if (m_iOldFrame != 3)
			{
				m_iOldFrame = 3;
				m_tInfo.fX += 12.f;
				m_tInfo.fY -= 9.f;
				m_tFrame.dwFrameSpeed = 50;
			}
		}
		else if (m_tFrame.iFrameCnt == 4)
		{
			if (m_iOldFrame != 4)
			{
				m_iOldFrame = 4;
				m_tInfo.fX += 9.f;
				m_tInfo.fY -= 12.f;
				m_tFrame.dwFrameSpeed = 50;
			}
		}
		else if (m_tFrame.iFrameCnt == 5)
		{
			if (m_iOldFrame != 5)
			{
				m_iOldFrame = 5;
				m_tInfo.fX += 6.f;
				m_tInfo.fY -= 6.f;
				m_tFrame.dwFrameSpeed = 50;
			}

			m_eCurState = CRobotBlue::IDLE_R;
			return true;
		}
	}

	return false;
}