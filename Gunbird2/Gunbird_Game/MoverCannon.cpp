// �����
// �̵� ĳ��
// ��ġ ������ ���� ��ġ �� ���� ����
// �ı��Ǹ� ���� ������ ������ ��������Ʈ�� �ı��� ���ط� ����

#include "stdafx.h"
#include "MoverCannon.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_02_1.h"
#include "Bullet_2.h"
#include "Coin.h"


CMoverCannon::CMoverCannon()
	: m_ePreState(CMoverCannon::END), m_eCurState(CMoverCannon::END), m_fDestPosX(0.f), m_fDestPosY(0.f),
	m_dwShotTime(GetTickCount()), m_dwShotDelay(0)
{
}


CMoverCannon::~CMoverCannon()
{
	Release();
}

void CMoverCannon::Initialize()
{
	// �⺻ ��ġ �� ����
	m_tInfo.iCX = MOVER_CANNON_WIDTH * 3;
	m_tInfo.iCY = MOVER_CANNON_HEIGHT * 3;
	m_tHitRectPos = { 6 * 3, 1 * 3, 26 * 3, 33 * 3 };
	m_iImageWidth = MOVER_CANNON_WIDTH;
	m_iImageHeight = MOVER_CANNON_HEIGHT;
	// �̵� �ӵ�
	m_fSpeed = 6.0f;
	// ü��
	m_fHp = 60.f;
	// �̹��� ��ȣ
	m_iDrawID = CMoverCannon::A90;
	// �⺻ ����
	m_eCurState = CMoverCannon::IDEL;
	// ���� ����
	m_dwShotDelay = 2000;

	// ����
	m_iScore = 600;
}

// ������Ʈ
int CMoverCannon::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;
	// ����
	if (m_bDead)
	{
		if (m_ePreState != CMoverCannon::DESTROY)
		{
			m_eCurState = CMoverCannon::DESTROY;
			m_bHpLock = true;		// �ı��Ǽ� HP �ٽ� ��
			CObj* pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX, m_tInfo.fY);		// �ı� ����Ʈ ����
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			// ������ ���� ����
			pObj = CObjFactory<CCoin>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);

			// �浹 ��Ʈ
			m_tHitRectPos = { 0,0,0,0 };
		}
	}

	// ��濡 ���缭 Y�� �̵�
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;
	// �������� ��� �ӵ��� ���缭 Y�� �̵��ؾߵ�
	m_fDestPosY += g_fBackgroundSpeed * 3.f;

	// �ı����� �ʾ���
	if (m_ePreState != CMoverCannon::DESTROY) {
		// ����
		Aim();
		// ����� ���� ��� �ð��� ��������
		if (m_dwShotTime + m_dwShotDelay < GetTickCount()) {
			// ����
			Shot();
			m_dwShotTime = GetTickCount();
		}
	}

	// �⺻ ����
	if (m_ePreState == CMoverCannon::IDEL)
	{
		// �̵�
		if (Move()) {
			// �̵� �Ϸ�
			m_eCurState = CMoverCannon::ON;
			// ���� �ı� �� �� ����
			m_bHpLock = false;
		}
	}

	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();
	// ������ �� ���� ó��
	Scene_Change();
	// �ִϸ��̼� ������ �̵�
	Frame_Move();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CMoverCannon::Late_Update()
{
	// �� �ϴ� ������ ������ ����
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
void CMoverCannon::Render(HDC _DC)
{
	HDC hMemDC;
	if (m_ePreState == CMoverCannon::IDEL || m_ePreState == CMoverCannon::ON)
	{
		// ��ü
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MoverCannon_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// ��ü
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MoverCannon_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_iDrawID * m_iImageWidth, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
	}
	if (m_ePreState == CMoverCannon::DESTROY)
	{
		// ����
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MoverCannon_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
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

void CMoverCannon::Release()
{
}

// ������ �� ���� ó��
void CMoverCannon::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			// ���
		case CMoverCannon::IDEL:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
			// ���⼭ �������� ���� Aim()���� ������
		case CMoverCannon::ON:
			break;
			// �ı�
		case CMoverCannon::DESTROY:
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

// �̵�
bool CMoverCannon::Move()
{
	float fX = m_fDestPosX - m_tInfo.fX;	// ���� ��ǥ X
	float fY = m_fDestPosY - m_tInfo.fY;	// ���� ��ǥ Y
	float fDia = sqrtf(fX * fX + fY * fY);	// ���� ��ġ���� ���������� �Ÿ�
	float fSpeed = m_fSpeed;				// �̵� �ӵ�

	// ������������ �Ÿ��� �̵� �ӵ����� �۴ٸ�
	// ��ǥ �Ÿ���ŭ �̵��� �� �ְ� �ӵ� ���� �ٲ�
	if (fDia < fSpeed)
		fSpeed = fDia;
	// ��ǥ �̵�
	m_tInfo.fX += fX / fDia * fSpeed;
	m_tInfo.fY += fY / fDia * fSpeed;
	
	// �������� �����ߴ���?
	if (fDia - fSpeed == 0)
		return true;

	return false;
}

// ����
void CMoverCannon::Aim()
{
	float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;	// ���� ��ǥ X
	float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;	// ���� ��ǥ Y
	float	fDia = sqrtf(fX * fX + fY * fY);			// ���� ��ġ���� ���������� �Ÿ�
	// arccos(cos) = ���� ���� ����
	float	fRad = acosf(fX / fDia);
	// ���� ������ ȣ������ ����
	m_fAngle = fRad * 180.f / PI;
	// Ÿ���� Y ��ǥ�� ���� Y ��ǥ ���� ũ�� ��ȣ ����
	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		m_fAngle *= -1.f;

	// ������ ���� ȣ�������� ��������Ʈ �ε��� ��ȣ ����
	if (-5.f <= m_fAngle && m_fAngle < 5.f)
		m_iDrawID = CMoverCannon::A0;
	if (-10.f <= m_fAngle && m_fAngle < -5.f)
		m_iDrawID = CMoverCannon::A_5;
	else if (-20.f <= m_fAngle && m_fAngle < -10.f)
		m_iDrawID = CMoverCannon::A_15;
	else if (-30.f <= m_fAngle && m_fAngle < -20.f)
		m_iDrawID = CMoverCannon::A_30;
	else if (-45.f <= m_fAngle && m_fAngle < -30.f)
		m_iDrawID = CMoverCannon::A_45;
	else if (-60.f <= m_fAngle && m_fAngle < -45.f)
		m_iDrawID = CMoverCannon::A_60;
	else if (-75.f <= m_fAngle && m_fAngle < -60.f)
		m_iDrawID = CMoverCannon::A_75;
	else if (-85.f <= m_fAngle && m_fAngle < -75.f)
		m_iDrawID = CMoverCannon::A_85;
	else if (-95.f <= m_fAngle && m_fAngle < -85.f)
		m_iDrawID = CMoverCannon::A_90;
	else if (-105.f <= m_fAngle && m_fAngle < -95.f)
		m_iDrawID = CMoverCannon::A_95;
	else if (-120.f <= m_fAngle && m_fAngle < -105.f)
		m_iDrawID = CMoverCannon::A_105;
	else if (-135.f <= m_fAngle && m_fAngle < -120.f)
		m_iDrawID = CMoverCannon::A_120;
	else if (-150.f <= m_fAngle && m_fAngle < -135.f)
		m_iDrawID = CMoverCannon::A_135;
	else if (-160.f <= m_fAngle && m_fAngle < -150.f)
		m_iDrawID = CMoverCannon::A_150;
	else if (-170.f <= m_fAngle && m_fAngle < -160.f)
		m_iDrawID = CMoverCannon::A_165;
	else if (-175.f <= m_fAngle && m_fAngle < -170.f)
		m_iDrawID = CMoverCannon::A_175;
	else if (-175.f < m_fAngle && 175.f <= m_fAngle )
		m_iDrawID = CMoverCannon::A180;
	else if (170.f <= m_fAngle && m_fAngle < 175.f)
		m_iDrawID = CMoverCannon::A175;
	else if (160.f <= m_fAngle && m_fAngle < 170.f)
		m_iDrawID = CMoverCannon::A165;
	else if (150.f <= m_fAngle && m_fAngle < 160.f)
		m_iDrawID = CMoverCannon::A150;
	else if (135.f <= m_fAngle && m_fAngle < 150.f)
		m_iDrawID = CMoverCannon::A135;
	else if (120.f <= m_fAngle && m_fAngle < 135.f)
		m_iDrawID = CMoverCannon::A120;
	else if (105.f <= m_fAngle && m_fAngle < 120.f)
		m_iDrawID = CMoverCannon::A105;
	else if (95.f <= m_fAngle && m_fAngle < 105.f)
		m_iDrawID = CMoverCannon::A95;
	else if (85.f <= m_fAngle && m_fAngle < 95.f)
		m_iDrawID = CMoverCannon::A90;
	else if (75.f <= m_fAngle && m_fAngle < 85.f)
		m_iDrawID = CMoverCannon::A85;
	else if (60.f <= m_fAngle && m_fAngle < 75.f)
		m_iDrawID = CMoverCannon::A75;
	else if (45.f <= m_fAngle && m_fAngle < 60.f)
		m_iDrawID = CMoverCannon::A60;
	else if (30.f <= m_fAngle && m_fAngle < 45.f)
		m_iDrawID = CMoverCannon::A45;
	else if (20.f <= m_fAngle && m_fAngle < 30.f)
		m_iDrawID = CMoverCannon::A30;
	else if (10.f <= m_fAngle && m_fAngle < 20.f)
		m_iDrawID = CMoverCannon::A15;
	else if (5.f <= m_fAngle && m_fAngle < 10.f)
		m_iDrawID = CMoverCannon::A5;
}

// ����
void CMoverCannon::Shot()
{
	// �Ѿ� ������Ʈ ����
	CObj* pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX, m_tInfo.fY);
	pObj->Set_Angle(m_fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}
