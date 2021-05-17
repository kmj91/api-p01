#include "stdafx.h"
#include "CG_TextWindow.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"


CCG_TextWindow::CCG_TextWindow()
	: m_eState(CCG_TextWindow::END)
	, m_dwTime(0), m_dwDelay(0), m_tStandingInfo{}, m_tStandingRect{}, m_byBlend(0)
{
}


CCG_TextWindow::~CCG_TextWindow()
{
	Release();
}

void CCG_TextWindow::Initialize()
{
	m_tInfo.fX = WINCX >> 1;
	m_tInfo.fY = WINCY - (55.f * 3.f);
	m_tInfo.iCX = CG_TEXT_BOX_WIDTH;
	m_tInfo.iCY = CG_TEXT_BOX_HEIGHT;
	m_iImageWidth = CG_TEXT_BOX_WIDTH;
	m_iImageHeight = CG_TEXT_BOX_HEIGHT;

	m_eState = CCG_TextWindow::ON_TEXTBOX;
	m_dwTime = GetTickCount();
	m_dwDelay = 1000;

	m_tStandingInfo[0].iCX = CG_MARION_STANDING_WIDTH * 3;
	m_tStandingInfo[0].iCY = CG_MARION_STANDING_HEIGHT * 3;
	m_tStandingInfo[1].iCX = CG_USAGI_STANDING_WIDTH * 3;
	m_tStandingInfo[1].iCY = CG_USAGI_STANDING_HEIGHT * 3;
}

int CCG_TextWindow::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	switch (m_eState)
	{
	case CCG_TextWindow::ON_TEXTBOX:
		// 투명도 변경
		if (m_byBlend + 5 <= 125) {
			m_byBlend = m_byBlend + 5;
		}

		if (m_dwTime + m_dwDelay < GetTickCount())
		{
			m_eState = CCG_TextWindow::TEXT_1;
			
			m_tStandingInfo[0].fX = (WINCX >> 1) + (-14.f * 3.f);
			m_tStandingInfo[0].fY = WINCY >> 1;

			m_dwDelay = 6000;
			m_dwTime = GetTickCount();
		}
		break;
	case CCG_TextWindow::TEXT_1:
		if (m_dwTime + m_dwDelay < GetTickCount())
		{
			m_eState = CCG_TextWindow::TEXT_2;

			m_tStandingInfo[1].fX = (WINCX >> 1) + (67.f * 3.f);
			m_tStandingInfo[1].fY = WINCY >> 1;

			m_dwTime = GetTickCount();
		}
		break;
	case CCG_TextWindow::TEXT_2:
		if (m_dwTime + m_dwDelay < GetTickCount())
		{
			m_eState = CCG_TextWindow::GAME_END;

			m_dwTime = GetTickCount();
		}
		break;
	case CCG_TextWindow::GAME_END:
		if (m_dwTime + m_dwDelay < GetTickCount())
		{
			g_bStageClear = true;
		}
		break;
	}

	

	Update_Rect();
	Update_RectEX();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CCG_TextWindow::Late_Update()
{
}

void CCG_TextWindow::Render(HDC _DC)
{
	HDC hMemDC;
	
	switch (m_eState)
	{
	case CCG_TextWindow::ON_TEXTBOX:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_TextBox");
		AlphaBlend(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, { AC_SRC_OVER, 0, m_byBlend, 0 });
		break;
	case CCG_TextWindow::TEXT_1:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Marion_01");
		GdiTransparentBlt(_DC
			, m_tStandingRect[0].left, m_tStandingRect[0].top
			, m_tStandingInfo[0].iCX, m_tStandingInfo[0].iCY
			, hMemDC
			, 0, 0
			, CG_MARION_STANDING_WIDTH, CG_MARION_STANDING_HEIGHT
			, RGB(255, 0, 255));

		// 반투명한 텍스트 박스
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_TextBox");
		AlphaBlend(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, { AC_SRC_OVER, 0, m_byBlend, 0 });

		// 텍스트
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Text_Stage_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case CCG_TextWindow::TEXT_2:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Marion_01");
		GdiTransparentBlt(_DC
			, m_tStandingRect[0].left, m_tStandingRect[0].top
			, m_tStandingInfo[0].iCX, m_tStandingInfo[0].iCY
			, hMemDC
			, 0, 0
			, CG_MARION_STANDING_WIDTH, CG_MARION_STANDING_HEIGHT
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Usagi_01");
		GdiTransparentBlt(_DC
			, m_tStandingRect[1].left, m_tStandingRect[1].top
			, m_tStandingInfo[1].iCX, m_tStandingInfo[1].iCY
			, hMemDC
			, 0, 0
			, CG_USAGI_STANDING_WIDTH, CG_USAGI_STANDING_HEIGHT
			, RGB(255, 0, 255));

		// 반투명한 텍스트 박스
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_TextBox");
		AlphaBlend(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, { AC_SRC_OVER, 0, m_byBlend, 0 });

		// 텍스트
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Text_Stage_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_iImageWidth, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case CCG_TextWindow::GAME_END:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Marion_01");
		GdiTransparentBlt(_DC
			, m_tStandingRect[0].left, m_tStandingRect[0].top
			, m_tStandingInfo[0].iCX, m_tStandingInfo[0].iCY
			, hMemDC
			, 0, 0
			, CG_MARION_STANDING_WIDTH, CG_MARION_STANDING_HEIGHT
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Usagi_01");
		GdiTransparentBlt(_DC
			, m_tStandingRect[1].left, m_tStandingRect[1].top
			, m_tStandingInfo[1].iCX, m_tStandingInfo[1].iCY
			, hMemDC
			, 0, 0
			, CG_USAGI_STANDING_WIDTH, CG_USAGI_STANDING_HEIGHT
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_TheEnd");
		GdiTransparentBlt(_DC
			, 0, 0
			, PUBLIC_BMP_WIDTH * 3, PUBLIC_BMP_HEIGHT * 3
			, hMemDC
			, 0, 0
			, PUBLIC_BMP_WIDTH, PUBLIC_BMP_HEIGHT
			, RGB(255, 0, 255));
		break;
	}
}

void CCG_TextWindow::Release()
{

}

void CCG_TextWindow::Update_RectEX()
{
	for (int i = 0; i < 2; ++i)
	{
		m_tStandingRect[i].left = (LONG)(m_tStandingInfo[i].fX - (m_tStandingInfo[i].iCX >> 1));
		m_tStandingRect[i].top = (LONG)(m_tStandingInfo[i].fY - (m_tStandingInfo[i].iCY >> 1));
		m_tStandingRect[i].right = (LONG)(m_tStandingInfo[i].fX + (m_tStandingInfo[i].iCX >> 1));
		m_tStandingRect[i].bottom = (LONG)(m_tStandingInfo[i].fY + (m_tStandingInfo[i].iCY >> 1));
	}
}
