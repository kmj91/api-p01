#include "stdafx.h"
#include "Stage1_BossTextBox.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Sector.h"
#include "RemoveSector.h"


CStage1_BossTextBox::CStage1_BossTextBox()
	: m_ePreState(CStage1_BossTextBox::END), m_eCurState(CStage1_BossTextBox::END),
	m_dwTime(0), m_dwDelay(0), m_dwCreateTime(0), m_dwCreateDelay(0), m_tStandingInfo{}, m_tStandingRect{}
{
}


CStage1_BossTextBox::~CStage1_BossTextBox()
{
	Release();
}

void CStage1_BossTextBox::Initialize()
{
	m_tInfo.fX = WINCX >> 1;
	m_tInfo.fY = WINCY - 150.f;
	m_tInfo.iCX = TEXT_BOX_WIDTH * 3;
	m_tInfo.iCY = TEXT_BOX_HEIGHT * 3;
	m_iImageWidth = TEXT_BOX_WIDTH;
	m_iImageHeight = TEXT_BOX_HEIGHT;

	m_eCurState = CStage1_BossTextBox::ON_TEXTBOX;
	m_dwDelay = 3000;
	m_dwCreateDelay = 1000;

	m_tStandingInfo[0].iCX = SHARK_STANDING_WIDTH * 3;
	m_tStandingInfo[0].iCY = SHARK_STANDING_HEIGHT * 3;
	m_tStandingInfo[1].iCX = GIMMICK_STANDING_WIDTH * 3;
	m_tStandingInfo[1].iCY = GIMMICK_STANDING_HEIGHT * 3;
	m_tStandingInfo[2].iCX = BLADE_STANDING_WIDTH * 3;
	m_tStandingInfo[2].iCY = BLADE_STANDING_HEIGHT * 3;
}

int CStage1_BossTextBox::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	switch (m_ePreState)
	{
	case CStage1_BossTextBox::ON_TEXTBOX:
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
			m_tFrame.dwFrameSpeed = 999999;
			m_eCurState = CStage1_BossTextBox::ACTION_1;
			
			m_tStandingInfo[0].fX = (float)(WINCX + (m_tStandingInfo[0].iCX >> 1));
			m_tStandingInfo[0].fY = (float)(WINCY - (m_tStandingInfo[0].iCY >> 1));

			m_dwTime = GetTickCount();		// 텍스트 박스 보여지는 시간
		}
		break;
	case CStage1_BossTextBox::ACTION_1:

		if (m_tStandingInfo[0].fX > (WINCX >> 1))
		{
			m_tStandingInfo[0].fX -= 15.f;
		}

		if (m_dwTime + m_dwDelay < GetTickCount())
		{
			m_eCurState = CStage1_BossTextBox::ACTION_2;
			m_dwTime = GetTickCount();		// 텍스트 박스 보여지는 시간

			m_tStandingInfo[1].fX = (float)(-(m_tStandingInfo[1].iCX >> 1));
			m_tStandingInfo[1].fY = (float)(WINCY - (m_tStandingInfo[1].iCY >> 1) - 50);
			m_tStandingInfo[2].fX = (float)(-(m_tStandingInfo[2].iCX >> 1));
			m_tStandingInfo[2].fY = (float)(WINCY - (m_tStandingInfo[2].iCY >> 1));
		}
		break;
	case CStage1_BossTextBox::ACTION_2:
		if (m_tStandingInfo[0].fX < (WINCX - 150))
		{
			m_tStandingInfo[0].fX += 15.f;
		}
		if (m_tStandingInfo[1].fX < (WINCX >> 1) - 150)
		{
			m_tStandingInfo[1].fX += 15.f;
		}
		if (m_tStandingInfo[2].fX < (WINCX >> 1) - 150)
		{
			m_tStandingInfo[2].fX += 15.f;
		}

		if (m_dwTime + m_dwDelay < GetTickCount())
		{
			m_eCurState = CStage1_BossTextBox::MOVE_BlADE;

			CObj* pObj = CAbstractFactory<CSector>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::SECTOR);
			pObj->Set_Pos(285, 400);				// 리스폰 위치
			pObj->Set_Size(30, 30);			// 크기
			static_cast<CSector*>(pObj)->Set_DestPos(50.f, 0.f);	// 이동 위치

			pObj = CAbstractFactory<CSector>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::SECTOR);
			pObj->Set_Pos(330, 400);				// 리스폰 위치
			pObj->Set_Size(5, 30);			// 크기
			static_cast<CSector*>(pObj)->Set_DestPos(0.f, -150.f);	// 이동 위치

			pObj = CAbstractFactory<CRemoveSector>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::REMOVE_SECTOR);
			pObj->Set_Pos(330, 250);				// 리스폰 위치
			pObj->Set_Size(30, 30);			// 크기

			m_dwCreateTime = GetTickCount();
		}
		break;
	case CStage1_BossTextBox::MOVE_BlADE:
		if (m_dwCreateTime + m_dwCreateDelay < GetTickCount())
		{
			m_eCurState = CStage1_BossTextBox::MOVE_SHARK;

			CObj* pObj = CAbstractFactory<CSector>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::SECTOR);
			pObj->Set_Pos(375, 400);				// 리스폰 위치
			pObj->Set_Size(30, 30);			// 크기
			static_cast<CSector*>(pObj)->Set_DestPos(-150.f, 0.f);	// 이동 위치

			m_dwCreateTime = GetTickCount();
			m_dwCreateDelay = 1500;
		}
	case CStage1_BossTextBox::MOVE_SHARK:
		if (m_dwCreateTime + m_dwCreateDelay < GetTickCount())
		{
			m_eCurState = CStage1_BossTextBox::BOSS_GO;

			CObj* pObj = CAbstractFactory<CSector>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::SECTOR);
			pObj->Set_Pos(330, 430);				// 리스폰 위치
			pObj->Set_Size(30, 30);			// 크기
			static_cast<CSector*>(pObj)->Set_DestPos(0.f, -150.f);	// 이동 위치

			m_dwTime = GetTickCount();
			m_dwDelay = 2000;
		}
		break;
	case CStage1_BossTextBox::BOSS_GO:
		if (m_dwTime + m_dwDelay < GetTickCount())
		{
			CObjMgr::Get_Instance()->Delete_ID(OBJID::REMOVE_SECTOR);
			CObjMgr::Get_Instance()->Delete_ID(OBJID::SECTOR);

			g_bStopMapScroll = false;		// 스크롤 다시 움직임
			g_bBossGO = true;
		}
		break;
	}

	

	Update_Rect();
	Update_RectEX();
	Scene_Change();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CStage1_BossTextBox::Late_Update()
{
}

void CStage1_BossTextBox::Render(HDC _DC)
{
	HDC hMemDC;

	switch (m_ePreState)
	{
	case CStage1_BossTextBox::ON_TEXTBOX:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TextBox");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case CStage1_BossTextBox::ACTION_1:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shark_Standing");
		GdiTransparentBlt(_DC
			, m_tStandingRect[0].left, m_tStandingRect[0].top
			, m_tStandingInfo[0].iCX, m_tStandingInfo[0].iCY
			, hMemDC
			, 0, 0
			, SHARK_STANDING_WIDTH, SHARK_STANDING_HEIGHT
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BossText_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case CStage1_BossTextBox::ACTION_2:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Gimmick_Standing");
		GdiTransparentBlt(_DC
			, m_tStandingRect[1].left, m_tStandingRect[1].top
			, m_tStandingInfo[1].iCX, m_tStandingInfo[1].iCY
			, hMemDC
			, 0, 0
			, GIMMICK_STANDING_WIDTH, GIMMICK_STANDING_HEIGHT
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Blade_Standing");
		GdiTransparentBlt(_DC
			, m_tStandingRect[2].left, m_tStandingRect[2].top
			, m_tStandingInfo[2].iCX, m_tStandingInfo[2].iCY
			, hMemDC
			, 0, 0
			, BLADE_STANDING_WIDTH, BLADE_STANDING_HEIGHT
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shark_Standing");
		GdiTransparentBlt(_DC
			, m_tStandingRect[0].left, m_tStandingRect[0].top
			, m_tStandingInfo[0].iCX, m_tStandingInfo[0].iCY
			, hMemDC
			, 0, 0
			, SHARK_STANDING_WIDTH, SHARK_STANDING_HEIGHT
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BossText_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case CStage1_BossTextBox::MOVE_BlADE:
		break;
	case CStage1_BossTextBox::MOVE_SHARK:
		break;
	}
}

void CStage1_BossTextBox::Release()
{
}

void CStage1_BossTextBox::Update_RectEX()
{
	for (int i = 0; i < 3; ++i)
	{
		m_tStandingRect[i].left = (LONG)(m_tStandingInfo[i].fX - (m_tStandingInfo[i].iCX >> 1));
		m_tStandingRect[i].top = (LONG)(m_tStandingInfo[i].fY - (m_tStandingInfo[i].iCY >> 1));
		m_tStandingRect[i].right = (LONG)(m_tStandingInfo[i].fX + (m_tStandingInfo[i].iCX >> 1));
		m_tStandingRect[i].bottom = (LONG)(m_tStandingInfo[i].fY + (m_tStandingInfo[i].iCY >> 1));
	}
}

void CStage1_BossTextBox::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CStage1_BossTextBox::ON_TEXTBOX:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CStage1_BossTextBox::ACTION_1:
			break;
		case CStage1_BossTextBox::ACTION_2:
			break;
		case CStage1_BossTextBox::MOVE_BlADE:
			break;
		case CStage1_BossTextBox::MOVE_SHARK:
			break;
		}
		m_ePreState = m_eCurState;
	}
}
