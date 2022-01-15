// �����
// ������ ���� ���� ���� ����Ʈ

#include "stdafx.h"
#include "Effect_MarionMeleeHit.h"

#include "BmpMgr.h"


CEffect_MarionMeleeHit::CEffect_MarionMeleeHit()
{
}


CEffect_MarionMeleeHit::~CEffect_MarionMeleeHit()
{
	Release();
}

void CEffect_MarionMeleeHit::Initialize()
{
	// �̹��� ũ�� �ʱ�ȭ
	m_tInfo.iCX = EFFECT_MARIONMELEEHIT_WIDTH * 3;
	m_tInfo.iCY = EFFECT_MARIONMELEEHIT_HEIGHT * 3;
	m_iImageWidth = EFFECT_MARIONMELEEHIT_WIDTH;
	m_iImageHeight = EFFECT_MARIONMELEEHIT_HEIGHT;
	// �̹��� ������ �ʱ�ȭ
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

// ������Ʈ
int CEffect_MarionMeleeHit::Update()
{
	// ����
	if (m_bDead)
		return OBJ_DEAD;

	// ����Ʈ ������ �ð��� ������ �÷��� true
	if (!m_bOnEffect && m_dwEffectTime + m_dwEffectDelay > GetTickCount())
		return OBJ_NOEVENT;
	else
		m_bOnEffect = true;

	// �̹��� RECT ���� ����
	Update_Rect();
	// �̹��� ������ �̵�
	Frame_Move();
	// �������� ���̸� ����
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_bDead = true;

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CEffect_MarionMeleeHit::Late_Update()
{
}

// ����
void CEffect_MarionMeleeHit::Render(HDC _DC)
{
	// �÷��װ� true�� �ƴϸ� ������ ���� ����
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Effect_MarionMeleeHit");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CEffect_MarionMeleeHit::Release()
{
}
