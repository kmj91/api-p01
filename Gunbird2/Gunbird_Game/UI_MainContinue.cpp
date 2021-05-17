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

	// UI 출력
	if (m_bOnUI) {
		// 컨티뉴 시간 감소
		Update_Count();
	}

	return OBJ_NOEVENT;
}

void CUI_MainContinue::Late_Update()
{
}

void CUI_MainContinue::Render(HDC _DC)
{
	// UI 출력 상태가 아니면
	if (!m_bOnUI) {
		return;
	}

	// 고정 텍스트
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_MainContinue");
	GdiTransparentBlt(_DC
		, 0, 0
		, MAIN_CONTINUE_WIDTH * 3, MAIN_CONTINUE_HEIGHT * 3
		, hMemDC
		, 0, 0
		, MAIN_CONTINUE_WIDTH, MAIN_CONTINUE_HEIGHT
		, RGB(255, 0, 255));

	// 컨티뉴 숫자
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

// 메인 컨티뉴 카운트 시작
void CUI_MainContinue::Start_Countdown()
{
	// 기존 프레임 구조체를 사용
	// 하지만 Frame_Move() 함수는 사용하지 않습니다
	m_tFrame.iFrameCnt = 9;
	m_tFrame.iFrameStart = 9;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 2000;

	// UI 출력 플래그 true
	m_bOnUI = true;
}

// 메인 컨티뉴 카운트 정지
void CUI_MainContinue::Stop_Countdown()
{
	// UI 출력 플래그 true
	m_bOnUI = false;
}

// 카운트 감소
void CUI_MainContinue::NextCount()
{
	if (m_tFrame.iFrameCnt > 0) 
	{
		--m_tFrame.iFrameCnt;
		m_tFrame.dwFrameTime = GetTickCount();
	}
	else
	{
		// 게임 오버
		g_bGameOver = true;
		m_tFrame.iFrameCnt = m_tFrame.iFrameEnd;
	}
}

// 컨티뉴 시간 감소
void CUI_MainContinue::Update_Count()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		--m_tFrame.iFrameCnt;
		if (m_tFrame.iFrameCnt < m_tFrame.iFrameEnd) {
			// 게임 오버
			g_bGameOver = true;
			m_tFrame.iFrameCnt = m_tFrame.iFrameEnd;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
