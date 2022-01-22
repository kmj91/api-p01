// �����
// NPC ���̵� ��� ���
// ���� �÷��� �߰��� ����� ����

#include "stdafx.h"
#include "Blade.h"

#include "BmpMgr.h"
#include "ObjMgr.h"


CBlade::CBlade() : m_ePreState(CBlade::END), m_eCurState(CBlade::END)
{
}


CBlade::~CBlade()
{
	Release();
}

void CBlade::Initialize()
{
	// �̹��� ũ�� ����
	m_tInfo.iCX = BLADE_WIDTH * 3;
	m_tInfo.iCY = BLADE_HEIGHT * 3;
	m_iImageWidth = BLADE_WIDTH;
	m_iImageHeight = BLADE_HEIGHT;
	// �̵� �ӵ�
	m_fSpeed = 1.5f;
	// �⺻ ����
	m_eCurState = CBlade::IDLE;
}

// ������Ʈ
int CBlade::Update()
{
	// ����
	if (m_bDead)
		return OBJ_DEAD;

	// ��� ��ũ���� ���� ���°� �ƴϸ�
	if (!g_bStopMapScroll) {
		// ��濡 ���缭 Y�� �̵�
		m_tInfo.fY += g_fBackgroundSpeed * 3.f;
		// �������� ��� �ӵ��� ���缭 Y�� �̵��ؾߵ�
		m_fDestPosY += g_fBackgroundSpeed * 3.f;
	}

	// �̵�
	Move();

	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();
	// ������ �� ���� ó��
	Scene_Change();
	// �̹��� ������ �̵�
	Frame_Move();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CBlade::Late_Update()
{
}

// ����
void CBlade::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Blade_Move");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CBlade::Release()
{
}

// ������ �� ���� ó��
void CBlade::Scene_Change()
{
	// ���� ���¿� �ٸ��� ��ü
	if (m_ePreState != m_eCurState)
	{
		// �̵� ���⿡ ���� �̹��� ������ ��ü
		switch (m_eCurState)
		{
		case CBlade::MOVE_U:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_RU:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_RD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_D:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_LD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 5;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 6;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::MOVE_LU:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 7;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::IDLE_L:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 8;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::IDLE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 9;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::IDLE_R:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 10;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CBlade::IDLE_2:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 11;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// �̵�
void CBlade::Move()
{
	float fX = m_fDestPosX - m_tInfo.fX;	// ���� ��ǥ X
	float fY = m_fDestPosY - m_tInfo.fY;	// ���� ��ǥ Y
	float fDia = sqrtf(fX * fX + fY * fY);	// ���� ��ġ���� ���������� �Ÿ�
	float fSpeed = m_fSpeed;				// �̵� �ӵ�

	// ������������ �Ÿ��� �̵� �ӵ����� �۴ٸ�
	// ��ǥ �Ÿ���ŭ �̵��� �� �ְ� �ӵ� ���� �ٲ�
	if (fDia < fSpeed)
		fSpeed = fDia;
	// �������� �����ߴ���?
	if (fDia - fSpeed == 0) {
		// ��� ����
		m_eCurState = CBlade::IDLE;
		return;
	}
	// ��ǥ �̵�
	m_tInfo.fX += fX / fDia * fSpeed;
	m_tInfo.fY += fY / fDia * fSpeed;
	// arccos(cos) = ���� ���� ����
	float fRad = acosf(fX / fDia);
	// ���� ������ ������ ����
	float fAngle = fRad * 180.f / PI;
	// �������� Y ��ǥ�� ���� Y ��ǥ ���� ũ�� ��ȣ ����
	if (m_tInfo.fY < m_fDestPosY)
		fAngle *= -1.f;

	// ������ ���� �̵� ���� ��ü
	if ((int)fAngle == 90)
	{
		m_eCurState = CBlade::MOVE_U;
	}
	else if (0 < (int)fAngle && (int)fAngle < 90)
	{
		m_eCurState = CBlade::MOVE_RU;
	}
	else if ((int)fAngle == 0)
	{
		m_eCurState = CBlade::MOVE_R;
	}
	else if (-90 < (int)fAngle && (int)fAngle < 0)
	{
		m_eCurState = CBlade::MOVE_RD;
	}
	else if ((int)fAngle == -90)
	{
		m_eCurState = CBlade::MOVE_D;
	}
	else if (-180 < (int)fAngle && (int)fAngle < -90)
	{
		m_eCurState = CBlade::MOVE_LD;
	}
	else if ((int)fAngle == 180)
	{
		m_eCurState = CBlade::MOVE_L;
	}
	else if (90 < (int)fAngle && (int)fAngle < 180)
	{
		m_eCurState = CBlade::MOVE_LU;
	}
}
