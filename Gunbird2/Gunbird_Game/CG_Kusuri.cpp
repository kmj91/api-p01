// �����
// ���� ���� �� CG ������Ʈ
// �̹��� ������ ���� �����ϸ� �ؽ�Ʈ ������ ����

#include "stdafx.h"
#include "CG_Kusuri.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "CG_TextWindow.h"


CCG_Kusuri::CCG_Kusuri() : m_iBGFrameCnt(0)
{
}


CCG_Kusuri::~CCG_Kusuri()
{
	Release();
}

void CCG_Kusuri::Initialize()
{
	// �̹��� ũ�� �ʱ�ȭ
	m_tInfo.iCX = CG_KUSURI_WIDTH * 3;
	m_tInfo.iCY = CG_KUSURI_HEIGHT * 3;
	m_iImageWidth = CG_KUSURI_WIDTH;
	m_iImageHeight = CG_KUSURI_HEIGHT;
	// �⺻ �ִϸ��̼� ������ ����
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 40;
	// ��ǥ �ʱ�ȭ
	m_tInfo.fX = 113.f * 3.f;
	m_tInfo.fY = 65.f * 3.f;
}

int CCG_Kusuri::Update()
{
	// ����
	if (m_bDead)
		return OBJ_DEAD;

	// �̹��� ��ü
	if (m_tFrame.iFrameScene == 0 && m_tFrame.iFrameCnt == 9) {
		m_tFrame.iFrameCnt = 0;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 15;
		m_tFrame.iFrameScene = 1;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 100;

		m_iBGFrameCnt = 1;
	}

	if (!m_bFlag && m_tFrame.iFrameCnt == 15) {
		m_bFlag = true;
		// �ؽ�Ʈ ������ ����
		CObj* pObj = CAbstractFactory<CCG_TextWindow>::Create();
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);
	}

	Update_Rect();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CCG_Kusuri::Late_Update()
{
}

void CCG_Kusuri::Render(HDC _DC)
{
	// ���
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Kusuri_BG");
	GdiTransparentBlt(_DC
		, 0, 0
		, 224 * 3, 320 * 3
		, hMemDC
		, m_iBGFrameCnt * 224, 0
		, 224, 320
		, RGB(255, 0, 255));

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Kusuri_1");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CCG_Kusuri::Release()
{
}
