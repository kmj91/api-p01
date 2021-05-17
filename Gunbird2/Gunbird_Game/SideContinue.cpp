#include "stdafx.h"
#include "SideContinue.h"

#include "BmpMgr.h"


CSideContinue::CSideContinue()
	: m_dwCountdown(0)
{
}


CSideContinue::~CSideContinue()
{
	Release();
}

void CSideContinue::Initialize()
{
}

int CSideContinue::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CSideContinue::Late_Update()
{
}

void CSideContinue::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Number");

	GdiTransparentBlt(_DC
		, (int)m_tInfo.fX, (int)m_tInfo.fY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CSideContinue::Release()
{
}
