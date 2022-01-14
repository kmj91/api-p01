// �����
// �Ŀ��� ������
// ȭ�� �ȿ��� Ư�� ������ �ε����� ���ƴٴ�
// �ı��� ȣ��Ǹ� �Ŀ��� �ؽ�Ʈ ������Ʈ ����

#include "stdafx.h"
#include "PowerUp.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "UI_PowerUpDownText.h"


CPowerUp::CPowerUp() : m_fRadian(0.f)
{
}


CPowerUp::~CPowerUp()
{
	Release();
}

void CPowerUp::Initialize()
{
	// �̹��� ũ�� �� Hit RECT �ʱ�ȭ
	m_tInfo.iCX = POWER_UP_WIDTH * 3;
	m_tInfo.iCY = POWER_UP_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, POWER_UP_WIDTH * 3, POWER_UP_HEIGHT * 3 };
	m_iImageWidth = POWER_UP_WIDTH;
	m_iImageHeight = POWER_UP_HEIGHT;
	// �̹��� ������ �ʱ�ȭ
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;
	// �̵� �ӵ�
	m_fSpeed = 4.0f;
	// ���� ���� �ʱ�ȭ
	int iRadValue = rand() % 20;
	m_fAngle = -70.f + (float)iRadValue;
	m_fRadian = PI * m_fAngle / 180.f;
	// ������ Ÿ�� �ʱ�ȭ
	m_enItemType = ITEM::POWER_UP;
}

// ������Ʈ
int CPowerUp::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += cosf(m_fRadian) * m_fSpeed;
	m_tInfo.fY += -sinf(m_fRadian) * m_fSpeed;

	//---------------------------
	// ȭ�� ���� ������ �ʴ´�
	//---------------------------
	if (m_tInfo.fX < 0) {
		m_fRadian = PI - m_fRadian;
	}
	if (m_tInfo.fX > WINCX) {
		m_fRadian = PI - m_fRadian;
	}
	if (m_tInfo.fY < 0) {
		m_fRadian = 2 * PI - m_fRadian;
	}
	if (m_tInfo.fY > WINCY - 200) {
		m_fRadian = 2 * PI - m_fRadian;
	}

	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();
	// �̹��� ������ �̵�
	Frame_Move();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CPowerUp::Late_Update()
{
}

// ����
void CPowerUp::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PowerUp");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// �浹 �ڽ�
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CPowerUp::Release()
{
	// �Ŀ� �� �ؽ�Ʈ ���
	CObj* pObj = CAbstractFactory<CUI_PowerUpDownText>::Create(m_tInfo.fX, m_tInfo.fY);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);
}
