// �����
// ���� ������Ʈ
// ����� �Ϻ��̰ų� ���̸� ǥ���ϱ� ���� ������ ������Ʈ

#include "stdafx.h"
#include "StaticObj.h"

#include "BmpMgr.h"


CStaticObj::CStaticObj()
{
}


CStaticObj::~CStaticObj()
{
	Release();
}

void CStaticObj::Initialize()
{
}

// ������Ʈ
int CStaticObj::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	// ��濡 ���缭 Y�� �̵�
	if (!g_bStopMapScroll)
		m_tInfo.fY += g_fBackgroundSpeed * 3.f;

	// �̹��� RECT ���� ����
	// ����� �Ϻ��̱� ������ Hit RECT�� ���� ���ʿ�
	Update_RectEx();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CStaticObj::Late_Update()
{
	if (WINCY <= m_tRect.top)
		m_bDead = true;
}

// ����
void CStaticObj::Render(HDC _DC)
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

void CStaticObj::Release()
{
}

// �̹��� RECT ���� ����
// ����� �Ϻ��̱� ������ Hit RECT�� ���� ���ʿ�
// �̹����� ��ġ�� �� �̹��� ���� ��ǥ�� ��ġ�� ���ϰ� ���� ��� ���������� ��ġ��
// �׷��� ������ Update_Rect() �Լ��� ������� ����
void CStaticObj::Update_RectEx()
{
	m_tRect.left = (LONG)m_tInfo.fX;
	m_tRect.top = (LONG)m_tInfo.fY;
	m_tRect.right = (LONG)(m_tInfo.fX + m_tInfo.iCX);
	m_tRect.bottom = (LONG)(m_tInfo.fY + m_tInfo.iCY);
}
