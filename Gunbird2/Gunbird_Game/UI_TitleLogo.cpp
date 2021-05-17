#include "stdafx.h"
#include "UI_TitleLogo.h"

#include "BmpMgr.h"
#include "GameUIMgr.h"


CUI_TitleLogo::CUI_TitleLogo()
	: m_eState(CUI_TitleLogo::END)
	, m_dwTime(GetTickCount()), m_dwDelay(600), m_byBlend(255), m_bRander(false)
{
}


CUI_TitleLogo::~CUI_TitleLogo()
{
	Release();
}

void CUI_TitleLogo::Initialize()
{
	m_tInfo.iCX = PUBLIC_BMP_WIDTH * 3;
	m_tInfo.iCY = PUBLIC_BMP_HEIGHT * 3;
	m_iImageWidth = PUBLIC_BMP_WIDTH;
	m_iImageHeight = PUBLIC_BMP_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 38;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 40;

	m_fSpeed = 3.0f;

	m_eState = CUI_TitleLogo::ON_TITLE_LOGO;
	m_dwTime = GetTickCount();
	m_dwDelay = 1000;
}

int CUI_TitleLogo::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	switch (m_eState)
	{
	case CUI_TitleLogo::ON_TITLE_LOGO:
		if (m_tFrame.iFrameCnt == 38) {
			m_eState = CUI_TitleLogo::BLEND_LOGO;
		}
		break;
	case CUI_TitleLogo::BLEND_LOGO:
		// 투명도 변경
		if (m_byBlend - 5 >= 0) {
			m_byBlend = m_byBlend - 5;
		}
		else {
			m_eState = CUI_TitleLogo::MOVE_LOGO;

			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 500;
		}
		break;
	case CUI_TitleLogo::MOVE_LOGO:
		// 로고 이동
		if (m_tInfo.fY - 5.f >= -30.f * 3.f) {
			m_tInfo.fY = m_tInfo.fY - 5.f;
		}
		break;
	}

	Frame_Move();

	return OBJ_NOEVENT;
}

void CUI_TitleLogo::Late_Update()
{
}

void CUI_TitleLogo::Render(HDC _DC)
{
	HDC hMemDC;

	switch (m_eState)
	{
	case CUI_TitleLogo::ON_TITLE_LOGO:
		// 배경
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BG_Black");
		GdiTransparentBlt(_DC
			, 0, 0
			, 224 * 3, 320 * 3
			, hMemDC
			, 0, 0
			, 224, 320
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_TitleLogo_1");
		GdiTransparentBlt(_DC
			, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case CUI_TitleLogo::BLEND_LOGO:
		// 배경
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Kusuri_BG");
		GdiTransparentBlt(_DC
			, 0, 0
			, 224 * 3, 320 * 3
			, hMemDC
			, 0, 0
			, 224, 320
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BG_White");
		AlphaBlend(_DC
			, 0, 0
			, 224 * 3, 320 * 3
			, hMemDC
			, 0, 0
			, 224, 320
			, { AC_SRC_OVER, 0, m_byBlend, 0 });

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_TitleLogo_2");
		GdiTransparentBlt(_DC
			, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case CUI_TitleLogo::MOVE_LOGO:
		// 배경
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CG_Kusuri_BG");
		GdiTransparentBlt(_DC
			, 0, 0
			, 224 * 3, 320 * 3
			, hMemDC
			, 0, 0
			, 224, 320
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BG_White");
		AlphaBlend(_DC
			, 0, 0
			, 224 * 3, 320 * 3
			, hMemDC
			, 0, 0
			, 224, 320
			, { AC_SRC_OVER, 0, m_byBlend, 0 });

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_TitleLogo_2");
		GdiTransparentBlt(_DC
			, 0, m_tInfo.fY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 크레딧이 있으면 Start 버튼
		if (0 < CGameUIMgr::Get_Instance()->Get_Credit()) {
			// 깜빡임
			if (m_tFrame.iFrameCnt == 3) {
				break;
			}

			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Start");
			GdiTransparentBlt(_DC
				, (WINCX >> 1) - ((UI_START_WIDTH * 3) >> 1), WINCY - (80.f * 3.f)
				, UI_START_WIDTH * 3, UI_START_HEIGHT * 3
				, hMemDC
				, m_tFrame.iFrameCnt * UI_START_WIDTH, m_tFrame.iFrameScene * UI_START_HEIGHT
				, UI_START_WIDTH, UI_START_HEIGHT
				, RGB(255, 0, 255));
		}
		// 크레딧이 없으면 Insert Coins
		else {
			// 깜빡임
			if (m_dwTime + m_dwDelay < GetTickCount()) {
				m_bRander = !m_bRander;
				m_dwTime = GetTickCount();
			}

			if (m_bRander) {
				hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_InsertCoins");
				GdiTransparentBlt(_DC
					, (WINCX >> 1) - ((UI_INSERT_COINS_WIDTH * 3) >> 1), WINCY - (80.f * 3.f)
					, UI_INSERT_COINS_WIDTH * 3, UI_INSERT_COINS_HEIGHT * 3
					, hMemDC
					, 0, 0
					, UI_INSERT_COINS_WIDTH, UI_INSERT_COINS_HEIGHT
					, RGB(255, 0, 255));
			}
		}
		break;
	}
}

void CUI_TitleLogo::Release()
{
}
