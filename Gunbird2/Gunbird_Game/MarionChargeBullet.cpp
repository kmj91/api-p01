// �����
// ������ ���� ���� �Ѿ�

#include "stdafx.h"
#include "MarionChargeBullet.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "MarionChargeHitEffect.h"


CMarionChargeBullet::CMarionChargeBullet() : m_ePreState(CMarionChargeBullet::END), m_eCurState(CMarionChargeBullet::END)
{
}


CMarionChargeBullet::~CMarionChargeBullet()
{
	Release();
}

void CMarionChargeBullet::Initialize()
{
	// �̹��� ��ġ �� ũ�� �ʱ�ȭ
	m_tInfo.iCX = MARION_CHARGE_BULLET_WIDTH * 3;
	m_tInfo.iCY = MARION_CHARGE_BULLET_HEIGHT * 3;
	m_tHitRectPos = { 2 * 3, 2 * 3, 34 * 3, 36 * 3 };
	m_iImageWidth = MARION_CHARGE_BULLET_WIDTH;
	m_iImageHeight = MARION_CHARGE_BULLET_HEIGHT;
	// �̵� �ӵ�
	m_fSpeed = 1.f;
	// ü��(�����ϸ鼭 �� ü�¸�ŭ �Ѿ� ü�µ� ���� 0�̵Ǹ� �Ҹ�)
	m_fHp = 50.f;
	// ������
	m_fDamage = 500.f;
	// �⺻ ����
	m_eCurState = CMarionChargeBullet::START;
}

// ������Ʈ
int CMarionChargeBullet::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;
	// ����
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE_BULLET_HIT);
		CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeBulletHit.wav", CSoundMgr::PLAYER_CHARGE_BULLET_HIT);

		CObj* pObj = CAbstractFactory<CMarionChargeHitEffect>::Create(m_tInfo.fX, m_tInfo.fY - 100.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}

	// �̵�
	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	// ó������ ������ ���� Ŀ���鼭
	// ���� ũ�� ���� �̹��� �������� �ݺ� �̵��ӵ� ����
	if (m_ePreState == CMarionChargeBullet::START) {
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = CMarionChargeBullet::REPEAT;
			m_fSpeed = 10.f;
		}
	}

	// ������ �� ���� ó��
	Scene_Change();
	// �̹��� ������ �̵�
	Frame_Move();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CMarionChargeBullet::Late_Update()
{
	// �� ������Ʈ�� Update���� �����Ǽ� Update�� �ǳ� �ٰ� Late_Update�� �ϴ� ��찡 �߻���
	// �׷��� Late_Udpate���� ������
	// �̹��� RECT �� Hit RECT ���� ����
	Update_Rect();

	// �� �ٱ����� ������ ����
	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
void CMarionChargeBullet::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Charge_Bullet");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CMarionChargeBullet::Release()
{
}

// ������ �� ���� ó��
void CMarionChargeBullet::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CMarionChargeBullet::START:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
		case CMarionChargeBullet::REPEAT:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
		}
		m_ePreState = m_eCurState;
	}
}
