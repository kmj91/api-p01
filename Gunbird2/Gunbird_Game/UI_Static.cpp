// 기명준
// UI 고정
// 별다른 처리없이 초기화 받은 프레임키로 렌더만 함

#include "stdafx.h"
#include "UI_Static.h"

#include "BmpMgr.h"


CUI_Static::CUI_Static()
{
}


CUI_Static::~CUI_Static()
{
	Release();
}

void CUI_Static::Initialize()
{
}

int CUI_Static::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CUI_Static::Late_Update()
{
}

void CUI_Static::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC
		, m_tInfo.fX, m_tInfo.fY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CUI_Static::Release()
{
}
