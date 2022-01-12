// �����
// �̵��ϴ� ���� ������Ʈ
// ����� �Ϻ��̰ų� ���̸� ǥ���ϱ� ���� ������ ������Ʈ

#include "stdafx.h"
#include "MoveStatic.h"

#include "BmpMgr.h"


CMoveStatic::CMoveStatic()
{
}


CMoveStatic::~CMoveStatic()
{
	Release();
}

void CMoveStatic::Initialize()
{
}

// ������Ʈ
int CMoveStatic::Update()
{
	// ����
	if (m_bDead)
		return OBJ_DEAD;

	// ��濡 ���缭 Y�� �̵�
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;
	// ��� ��ũ�� ����� �����ؼ� ��ǥ Y�ຸ�� ũ��
	if (m_tInfo.fY > m_fDestPosY)
	{
		// �������� ��� �ӵ��� ���缭 Y�� �̵��ؾߵ�
		m_fDestPosY += g_fBackgroundSpeed * 3.f;

		float	fX = m_fDestPosX - m_tInfo.fX;		// ���� ��ǥ X
		float	fY = m_fDestPosY - m_tInfo.fY;		// ���� ��ǥ Y
		float	fDia = sqrtf(fX * fX + fY * fY);	// ���� ��ġ���� ���������� �Ÿ�
		float fSpeed = m_fSpeed;					// �̵� �ӵ�

		// ������������ �Ÿ��� �̵� �ӵ����� �۴ٸ�
		// ��ǥ �Ÿ���ŭ �̵��� �� �ְ� �ӵ� ���� �ٲ�
		if (fDia < fSpeed)
			fSpeed = fDia;

		// �������� �����ߴ���?
		if (fDia - fSpeed != 0) {
			// ��ǥ �̵�
			m_tInfo.fX += fX / fDia * fSpeed;
			m_tInfo.fY += fY / fDia * fSpeed;
		}
	}

	// �̹��� RECT ���� ����
	Update_RectEx();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CMoveStatic::Late_Update()
{
	if (WINCY <= m_tRect.top || WINCX <= m_tRect.left)
		m_bDead = true;
}

// ����
void CMoveStatic::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC
		, (int)m_tInfo.fX, (int)m_tInfo.fY			// ������ǥ���� left top�� x y �� ��
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CMoveStatic::Release()
{
}

// �̹��� RECT ���� ����
// �̹����� ��ġ�� �� �̹��� ���� ��ǥ�� ��ġ�� ���ϰ� ���� ��� ���������� ��ġ��
// �׷��� ������ Update_Rect() �Լ��� ������� ����
void CMoveStatic::Update_RectEx()
{
	m_tRect.left = (LONG)m_tInfo.fX;
	m_tRect.top = (LONG)m_tInfo.fY;
	m_tRect.right = (LONG)(m_tInfo.fX + m_tInfo.iCX);
	m_tRect.bottom = (LONG)(m_tInfo.fY + m_tInfo.iCY);
}
