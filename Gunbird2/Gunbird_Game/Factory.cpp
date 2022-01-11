// �����
// ����
// �ı��Ǹ� ��ź ������ ���

#include "stdafx.h"
#include "Factory.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_04.h"

#include "Bomb.h"


CFactory::CFactory()
	:m_ePreState(CFactory::END), m_eCurState(CFactory::END), m_bRemove(false)
{
}


CFactory::~CFactory()
{
	Release();
}

void CFactory::Initialize()
{
	// �⺻ ��ġ �� ����
	m_tInfo.iCX = FACTORY_WIDTH * 3;
	m_tInfo.iCY = FACTORY_HEIGHT * 3;
	m_tHitRectPos = { 19 * 3, 20 * 3, 89 * 3, 95 * 3 };
	m_iImageWidth = FACTORY_WIDTH;
	m_iImageHeight = FACTORY_HEIGHT;
	// ü��
	m_fHp = 500.f;
	// �⺻ ����
	m_eCurState = CFactory::IDLE;
	// ���� ����
	m_bHpLock = false;

	// ����
	m_iScore = 1000;
}

// ������Ʈ
int CFactory::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;
	// ����
	if (m_bDead)
	{
		if (m_ePreState != CFactory::DESTROY)
		{
			// �ı� ���·� ����
			m_eCurState = CFactory::DESTROY;
			// �ı��Ǽ� HP �ٽ� ��
			m_bHpLock = true;
			// �ı� ����Ʈ ����
			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			// ��ź ������ ���
			pObj = CAbstractFactory<CBomb>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);

			// �浹 ��Ʈ
			m_tHitRectPos = { 0,0,0,0 };
		}
	}

	// ��濡 ���缭 Y�� �̵�
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;

	// ��� ǥ���ǿ� ���� ����
	if (m_ePreState == CFactory::IDLE)
	{
		// ��ǥ Y�� ��ǥ�� �����ϸ�
		if (m_tInfo.fY > 260.f)
		{
			// ���� ����
			m_eCurState = CFactory::OPEN;
			// �����ִ� ������Ʈ �ı�
			CObjMgr::Get_Instance()->Dead_ID(OBJID::MAP_OBJECT);
		}
	}
	// ����
	else if (m_ePreState == CFactory::OPEN)
	{
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = CFactory::ATTACK;
		}
	}
	// ����
	else if (m_ePreState == CFactory::ATTACK)
	{
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = CFactory::CLOSED;
		}
	}
	// ����
	else if (m_ePreState == CFactory::CLOSED)
	{
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = CFactory::IDEL_END;
		}
	}

	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();
	// ������ �� ���� ó��
	Scene_Change();
	// �̹��� ������ �̵�
	Frame_Move();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CFactory::Late_Update()
{
	// �� �ϴ� ������ ������ ����
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
void CFactory::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Factory");
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
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CFactory::Release()
{
}

// ������ �� ���� ó��
void CFactory::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			// ��� ǥ���ǿ� ���� ����
		case CFactory::IDLE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
			// ����
		case CFactory::OPEN:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
			// ����
		case CFactory::ATTACK:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
			// ����
		case CFactory::CLOSED:
			m_tFrame.iFrameCnt = 9;
			m_tFrame.iFrameStart = 9;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CFactory::DESTROY:
			m_tFrame.iFrameCnt = 8;
			m_tFrame.iFrameStart = 8;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
		case CFactory::IDEL_END:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
		}
		m_ePreState = m_eCurState;
	}
}
