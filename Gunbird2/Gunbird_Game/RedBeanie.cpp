#include "stdafx.h"
#include "RedBeanie.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_02_1.h"
#include "Bullet_2.h"


CRedBeanie::CRedBeanie()
	: m_ePreState(CRedBeanie::END), m_eCurState(CRedBeanie::END), m_fDestPosY(0.f),
	m_dwShotTime(0), m_dwShotDelay(0), m_iShot(0), m_tBodyFrame{}
{
}


CRedBeanie::~CRedBeanie()
{
	Release();
}

void CRedBeanie::Initialize()
{
	// �⺻ ��ġ �� ����
	m_tInfo.iCX = RED_BEANIE_WIDTH * 3;
	m_tInfo.iCY = RED_BEANIE_HEIGHT * 3;
	m_tHitRectPos = { 18 * 3, 7 * 3, 41 * 3, 38 * 3 };
	m_iImageWidth = RED_BEANIE_WIDTH;
	m_iImageHeight = RED_BEANIE_HEIGHT;
	// �⺻ �ִϸ��̼� ������ ����
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 40;
	// �̵� �ӵ�
	m_fSpeed = 5.0f;
	// ü��
	m_fHp = 80.f;
	// �⺻ ����
	m_eCurState = CRedBeanie::IDLE;
	// ���� ����
	m_dwShotDelay = 200;
	// ���� Ƚ��
	m_iShot = 2;

	// ����
	m_iScore = 1000;
}

// ������Ʈ
int CRedBeanie::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;
	// ����
	if (m_bDead)
	{
		// �ı� ����Ʈ ����
		CObj* pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		return OBJ_DEAD;
	}
		

	// ���������� ���� �������� ��������
	if(m_fDestPosY > m_tInfo.fY)
		m_tInfo.fY += m_fSpeed;
	// ���������� ����
	else {
		// ���������� ������ ����
		if (m_ePreState == CRedBeanie::IDLE)
			m_eCurState = CRedBeanie::ATTACK;
		else if (m_ePreState == CRedBeanie::ATTACK) {
			// ���� ������ ����Ʈ 13 ���ĺ���
			if (m_tBodyFrame.iFrameCnt >= 13) {
				if (m_dwShotTime + m_dwShotDelay < GetTickCount()) {
					if (m_iShot > 0) {
						--m_iShot;
						Shot();
						m_dwShotTime = GetTickCount();
					}
				}
			}

			// ������ �������� ���¸� �̵����� ����
			if (m_tBodyFrame.iFrameCnt == m_tBodyFrame.iFrameEnd)
				m_eCurState = CRedBeanie::MOVE;
		}
		// �� �ϴ����� ����
		else if (m_ePreState == CRedBeanie::MOVE) {
			m_tInfo.fY += m_fSpeed;		 //Y�� �̵�
		}
	}

	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();
	// ������ �� ���� ó��
	Scene_Change();
	// �̹��� ������ �̵�
	Frame_Move();
	// ����Ʈ �̹��� ������ �̵�
	Frame_MoveEX();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CRedBeanie::Late_Update()
{
	// �� ������ ������ ������ ���� ���� �� ���� ����
	if (m_bHpLock)
		if (0 <= m_tRect.bottom)
			m_bHpLock = false;

	// �� �ϴ� ������ ������ ����
	if (WINCY + 100 <= m_tRect.top)
		m_bRemove = true;
}

// ����
void CRedBeanie::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RedBeanie_2");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RedBeanie_1");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tBodyFrame.iFrameCnt * m_iImageWidth, m_tBodyFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));


	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CRedBeanie::Release()
{
}

// ������ �� ���� ó��
void CRedBeanie::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			// ������ ���� �����ϱ� ��
		case CRedBeanie::IDLE:
			m_tBodyFrame.iFrameCnt = 0;
			m_tBodyFrame.iFrameStart = 0;
			m_tBodyFrame.iFrameEnd = 0;
			m_tBodyFrame.iFrameScene = 0;
			m_tBodyFrame.dwFrameTime = GetTickCount();
			m_tBodyFrame.dwFrameSpeed = 999999;
			break;
			// ���� �� ����
		case CRedBeanie::ATTACK:
			m_tBodyFrame.iFrameCnt = 0;
			m_tBodyFrame.iFrameStart = 0;
			m_tBodyFrame.iFrameEnd = 26;
			m_tBodyFrame.iFrameScene = 0;
			m_tBodyFrame.dwFrameTime = GetTickCount();
			m_tBodyFrame.dwFrameSpeed = 80;
			break;
			// ���� �Ϸ� �� ����
		case CRedBeanie::MOVE:
			m_tBodyFrame.iFrameCnt = 0;
			m_tBodyFrame.iFrameStart = 0;
			m_tBodyFrame.iFrameEnd = 0;
			m_tBodyFrame.iFrameScene = 0;
			m_tBodyFrame.dwFrameTime = GetTickCount();
			m_tBodyFrame.dwFrameSpeed = 999999;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// ����Ʈ �̹��� ������ �̵�
void CRedBeanie::Frame_MoveEX()
{
	if (m_tBodyFrame.dwFrameTime + m_tBodyFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tBodyFrame.iFrameCnt;
		if (m_tBodyFrame.iFrameCnt > m_tBodyFrame.iFrameEnd)
			m_tBodyFrame.iFrameCnt = m_tBodyFrame.iFrameStart;

		m_tBodyFrame.dwFrameTime = GetTickCount();
	}
}

// ����
void CRedBeanie::Shot()
{
	float fX = m_pTarget->Get_Info().fX - m_tInfo.fX;	// ���� ��ǥ X
	float fY = m_pTarget->Get_Info().fY - m_tInfo.fY;	// ���� ��ǥ Y
	float fDia = sqrtf(fX * fX + fY * fY);				// ���� ��ġ���� ���������� �Ÿ�
	// arccos(cos) = ���� ���� ����
	float fRad = acosf(fX / fDia);
	// ���� ������ ������ ����
	m_fAngle = fRad * 180.f / PI;
	// Ÿ���� Y ��ǥ�� ���� Y ��ǥ ���� ũ�� ��ȣ ����
	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		m_fAngle *= -1.f;

	// �Ѿ� ������Ʈ ����
	CObj* pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX, m_tInfo.fY);
	pObj->Set_Angle(m_fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}
