#include "stdafx.h"
#include "PlayerSelect.h"

#include "BmpMgr.h"


CPlayerSelect::CPlayerSelect()
	: m_ePreState(PLAYER::NAME_END), m_eCurState(PLAYER::NAME_END)
{
}


CPlayerSelect::~CPlayerSelect()
{
	Release();
}

void CPlayerSelect::Initialize()
{
	m_tInfo.fX = 150;
	m_tInfo.fY = 760;
	m_tInfo.iCX = 32;
	m_tInfo.iCY = 48;

	m_eCurState = PLAYER::MARION;
}

int CPlayerSelect::Update()
{
	Update_Rect();
	Scene_Change();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CPlayerSelect::Late_Update()
{
}

void CPlayerSelect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CharacterSelect");

	GdiTransparentBlt(_DC
		, 0, 0
		, 224 * 3, 320 * 3
		, hMemDC
		, m_tFrame.iFrameCnt * 224, m_tFrame.iFrameScene * 320
		, 224, 320
		, RGB(255, 0, 255));

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PlayerSelect");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX * 3, m_tInfo.iCY * 3
		, hMemDC
		, 0, 0
		, 32, 48
		, RGB(255, 0, 255));
}

void CPlayerSelect::Release()
{
}

void CPlayerSelect::Key_Input(int _Key)
{
	switch (_Key)
	{
	case VK_LEFT:
		if (m_eCurState - 1 < 0)
			m_eCurState = PLAYER::HEI_COB;
		else
			m_eCurState = (PLAYER::NAME)(m_eCurState - 1);

		m_tInfo.fX = 54.f + (32.f * 3 * m_eCurState);
		break;
	case VK_RIGHT:
		if (m_eCurState + 1 >= PLAYER::NAME_END)
			m_eCurState = PLAYER::ALUCARD;
		else
			m_eCurState = (PLAYER::NAME)(m_eCurState + 1);

		m_tInfo.fX = 54.f + (32.f * 3 * m_eCurState);
		break;
	}
}

PLAYER::NAME CPlayerSelect::Get_State()
{
	return m_ePreState;
}

void CPlayerSelect::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case PLAYER::ALUCARD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iFrameScene = PLAYER::ALUCARD;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 20;
			break;
		case PLAYER::MARION:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = PLAYER::MARION;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 50;
			break;
		case PLAYER::VALPIRO:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = PLAYER::VALPIRO;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 50;
			break;
		case PLAYER::TAVIA:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = PLAYER::TAVIA;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 50;
			break;
		case PLAYER::HEI_COB:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = PLAYER::HEI_COB;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 50;
			break;
		case PLAYER::NAME_END:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}
