// �����
// ���� ��ⱸ ����
// ���� ������ 2�� �������� ����
// ���� ��ġ�� ��������� ��ǥ ���� ����

#include "stdafx.h"
#include "PipeSmoke.h"

#include "BmpMgr.h"


CPipeSmoke::CPipeSmoke()
{
}


CPipeSmoke::~CPipeSmoke()
{
	Release();
}

void CPipeSmoke::Initialize()
{
	// �̹��� ũ�� �ʱ�ȭ
	m_tInfo.iCX = PIPE_SMOKE_WIDTH * 3;
	m_tInfo.iCY = PIPE_SMOKE_HEIGHT * 3;
	m_iImageWidth = PIPE_SMOKE_WIDTH;
	m_iImageHeight = PIPE_SMOKE_HEIGHT;
	// �̹��� ������ �ʱ�ȭ
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 15;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

// ������Ʈ
int CPipeSmoke::Update()
{
	// ����
	if (m_bDead)
		return OBJ_DEAD;

	// ����Ʈ ������ �ð��� ������ �÷��� true
	if (!m_bOnEffect && m_dwEffectTime + m_dwEffectDelay > GetTickCount())
		return OBJ_NOEVENT;
	else
		m_bOnEffect = true;

	// ��� ��ǥ�� ��ġ�� ����
	m_tInfo.fX = m_pTarget->Get_Info().fX + m_fRelativePosX;
	m_tInfo.fY = m_pTarget->Get_Info().fY + m_fRelativePosY;

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
void CPipeSmoke::Late_Update()
{
}

// ����
void CPipeSmoke::Render(HDC _DC)
{
	// �÷��װ� true�� �ƴϸ� ������ ���� ����
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PipeSmoke");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CPipeSmoke::Release()
{
}
