// �����
// ���� �ı� ����Ʈ
// ������ ���� ���� �����Ǽ� �����ų� ��

#include "stdafx.h"
#include "Explosion_05.h"

#include "BmpMgr.h"
#include "SoundMgr.h"


CExplosion_05::CExplosion_05()
{
}


CExplosion_05::~CExplosion_05()
{
	Release();
}

void CExplosion_05::Initialize()
{
	// �̹��� ũ�� �ʱ�ȭ
	m_tInfo.iCX = EXPLOSION_05_WIDTH * 3;
	m_tInfo.iCY = EXPLOSION_05_HEIGHT * 3;
	m_iImageWidth = EXPLOSION_05_WIDTH;
	m_iImageHeight = EXPLOSION_05_HEIGHT;
	// �̹��� ������ �ʱ�ȭ
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iFrameScene = rand() % 4;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
	// �̵� �ӵ�
	m_fSpeed = 15.f;
}

// ������Ʈ
int CExplosion_05::Update()
{
	// ����
	if (m_bDead)
		return OBJ_DEAD;
	// ����Ʈ ������ �ð��� ������ �÷��� true
	if (!m_bOnEffect && m_dwEffectTime + m_dwEffectDelay > GetTickCount())
		return OBJ_NOEVENT;
	else
		m_bOnEffect = true;

	// �Ʒ��� �̵�
	m_tInfo.fY += m_fSpeed;

	// �̹��� RECT ���� ����
	Update_Rect();
	// �̹��� ������ �̵�
	Frame_Move();
	// �������� ���̸� ����
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)		// �������� ���̸� �����
		m_bDead = true;

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CExplosion_05::Late_Update()
{
}

// ����
void CExplosion_05::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Explosion_05");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CExplosion_05::Release()
{
}
