#include "stdafx.h"
#include "UI_ChargeBar.h"

#include "BmpMgr.h"


CUI_ChargeBar::CUI_ChargeBar() : m_iCharge(0), m_iChargeLevel(0)
{
}


CUI_ChargeBar::~CUI_ChargeBar()
{
	Release();
}

void CUI_ChargeBar::Initialize()
{
	m_tInfo.iCX = UI_CHARGE_BAR_WIDTH * 3;
	m_tInfo.iCY = UI_CHARGE_BAR_HEIGHT * 3;
	m_iImageWidth = UI_CHARGE_BAR_WIDTH;
	m_iImageHeight = UI_CHARGE_BAR_HEIGHT;
}

int CUI_ChargeBar::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CUI_ChargeBar::Late_Update()
{
}

void CUI_ChargeBar::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_ChargeBar");

	GdiTransparentBlt(_DC
		, m_tInfo.fX, m_tInfo.fY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, (m_iCharge / 8) * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CUI_ChargeBar::Release()
{
}
