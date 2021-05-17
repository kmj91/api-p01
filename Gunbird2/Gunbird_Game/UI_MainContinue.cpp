#include "stdafx.h"
#include "UI_MainContinue.h"

#include "BmpMgr.h"


CUI_MainContinue::CUI_MainContinue()
	: m_bOnUI(false)
{
}


CUI_MainContinue::~CUI_MainContinue()
{
	Release();
}

void CUI_MainContinue::Initialize()
{
	m_tInfo.iCX = UI_BIG_NUMBER_WIDTH * 3;
	m_tInfo.iCY = UI_BIG_NUMBER_HEIGHT * 3;
	m_iImageWidth = UI_BIG_NUMBER_WIDTH;
	m_iImageHeight = UI_BIG_NUMBER_HEIGHT;
}

int CUI_MainContinue::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	// UI ���
	if (m_bOnUI) {
		// ��Ƽ�� �ð� ����
		Update_Count();
	}

	return OBJ_NOEVENT;
}

void CUI_MainContinue::Late_Update()
{
}

void CUI_MainContinue::Render(HDC _DC)
{
	// UI ��� ���°� �ƴϸ�
	if (!m_bOnUI) {
		return;
	}

	// ���� �ؽ�Ʈ
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_MainContinue");
	GdiTransparentBlt(_DC
		, 0, 0
		, MAIN_CONTINUE_WIDTH * 3, MAIN_CONTINUE_HEIGHT * 3
		, hMemDC
		, 0, 0
		, MAIN_CONTINUE_WIDTH, MAIN_CONTINUE_HEIGHT
		, RGB(255, 0, 255));

	// ��Ƽ�� ����
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_BigNumber");
	GdiTransparentBlt(_DC
		, (int)m_tInfo.fX, (int)m_tInfo.fY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CUI_MainContinue::Release()
{
}

// ���� ��Ƽ�� ī��Ʈ ����
void CUI_MainContinue::Start_Countdown()
{
	// ���� ������ ����ü�� ���
	// ������ Frame_Move() �Լ��� ������� �ʽ��ϴ�
	m_tFrame.iFrameCnt = 9;
	m_tFrame.iFrameStart = 9;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 2000;

	// UI ��� �÷��� true
	m_bOnUI = true;
}

// ���� ��Ƽ�� ī��Ʈ ����
void CUI_MainContinue::Stop_Countdown()
{
	// UI ��� �÷��� true
	m_bOnUI = false;
}

// ī��Ʈ ����
void CUI_MainContinue::NextCount()
{
	if (m_tFrame.iFrameCnt > 0) 
	{
		--m_tFrame.iFrameCnt;
		m_tFrame.dwFrameTime = GetTickCount();
	}
	else
	{
		// ���� ����
		g_bGameOver = true;
		m_tFrame.iFrameCnt = m_tFrame.iFrameEnd;
	}
}

// ��Ƽ�� �ð� ����
void CUI_MainContinue::Update_Count()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		--m_tFrame.iFrameCnt;
		if (m_tFrame.iFrameCnt < m_tFrame.iFrameEnd) {
			// ���� ����
			g_bGameOver = true;
			m_tFrame.iFrameCnt = m_tFrame.iFrameEnd;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
