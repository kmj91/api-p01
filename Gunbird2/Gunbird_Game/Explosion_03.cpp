// �����
// ���� �ı� ����Ʈ
// ������ ���� ���� �����Ǽ� �����ų� ��

#include "stdafx.h"
#include "Explosion_03.h"

#include "BmpMgr.h"
#include "SoundMgr.h"


CExplosion_03::CExplosion_03()
{
}


CExplosion_03::~CExplosion_03()
{
	Release();
}

void CExplosion_03::Initialize()
{
	// �̹��� ũ�� �ʱ�ȭ
	m_tInfo.iCX = EXPLOSION_03_WIDTH * 3;
	m_tInfo.iCY = EXPLOSION_03_HEIGHT * 3;
	m_iImageWidth = EXPLOSION_03_WIDTH;
	m_iImageHeight = EXPLOSION_03_HEIGHT;
	// �̹��� ������ �ʱ�ȭ
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 14;
	m_tFrame.iFrameScene = rand() % 4;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

// ������Ʈ
int CExplosion_03::Update()
{
	// ����
	if (m_bDead)
		return OBJ_DEAD;
	// ����Ʈ ������ �ð��� ������ �÷��� true
	if (!m_bOnEffect && m_dwEffectTime + m_dwEffectDelay > GetTickCount())
		return OBJ_NOEVENT;
	else {
		// �÷��� true
		m_bOnEffect = true;
		// ���� ���
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EXPLOSION_3);
		CSoundMgr::Get_Instance()->PlaySound(L"Explosion_03.wav", CSoundMgr::EXPLOSION_3);
	}

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
void CExplosion_03::Late_Update()
{
}

// ����
void CExplosion_03::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Explosion_03");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CExplosion_03::Release()
{
}
