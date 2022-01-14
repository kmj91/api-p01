// �����
// ������ ���� ���� ���� ����Ʈ

#include "stdafx.h"
#include "MarionChargeHitEffect.h"

#include "BmpMgr.h"


CMarionChargeHitEffect::CMarionChargeHitEffect()
{
}


CMarionChargeHitEffect::~CMarionChargeHitEffect()
{
	Release();
}

void CMarionChargeHitEffect::Initialize()
{
	// �̹��� ũ�� �ʱ�ȭ
	m_tInfo.iCX = MARION_CHARGE_BULLET_HIT_WIDTH * 3;
	m_tInfo.iCY = MARION_CHARGE_BULLET_HIT_HEIGHT * 3;
	m_iImageWidth = MARION_CHARGE_BULLET_HIT_WIDTH;
	m_iImageHeight = MARION_CHARGE_BULLET_HIT_HEIGHT;
	// �̹��� ������ �ʱ�ȭ
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 15;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 20;
}

// ������Ʈ
int CMarionChargeHitEffect::Update()
{
	// ����
	if (m_bDead)
		return OBJ_DEAD;
	// �̹��� ������ �̵�
	Frame_Move();
	// �������� ���̸� ����
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_bDead = true;

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CMarionChargeHitEffect::Late_Update()
{
	// �� ������Ʈ�� Update���� �����Ǽ� Update�� �ǳ� �ٰ� Late_Update�� �ϴ� ��찡 �߻���
	// �׷��� Late_Udpate���� ������
	// �̹��� RECT ���� ����
	Update_Rect();
}

// ����
void CMarionChargeHitEffect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Charge_Bullet_Hit");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CMarionChargeHitEffect::Release()
{
}
