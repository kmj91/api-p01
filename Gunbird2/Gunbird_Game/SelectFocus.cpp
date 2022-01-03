// 기명준
// 캐릭터 선택 표시 오브젝트
// 선택 포커스 조종 및 화면에서 선택한 캐릭터 표시

#include "stdafx.h"
#include "SelectFocus.h"

#include "BmpMgr.h"


CSelectFocus::CSelectFocus()
	: m_ePreCharacter(PLAYER::NAME_END), m_eCurCharacter(PLAYER::NAME_END)
{
}


CSelectFocus::~CSelectFocus()
{
	Release();
}

void CSelectFocus::Initialize()
{
	// 기본 포커스 좌표
	m_tInfo.fX = 150;
	m_tInfo.fY = 760;
	m_tInfo.iCX = 32;
	m_tInfo.iCY = 48;

	// 기본 선택 캐릭터
	m_eCurCharacter = PLAYER::MARION;
}

int CSelectFocus::Update()
{
	// 렉트 업데이트
	Update_Rect();
	// 캐릭터 교체
	CharacterChange();
	// 프레임 처리
	Frame_Move();

	return OBJ_NOEVENT;
}

void CSelectFocus::Late_Update()
{
}

void CSelectFocus::Render(HDC _DC)
{
	// 선택한 캐릭터 화면에 표시
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CharacterSelect");
	GdiTransparentBlt(_DC
		, 0, 0
		, 224 * 3, 320 * 3
		, hMemDC
		, m_tFrame.iFrameCnt * 224, m_tFrame.iFrameScene * 320
		, 224, 320
		, RGB(255, 0, 255));

	// 캐릭터 선택 포커스
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PlayerSelect");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX * 3, m_tInfo.iCY * 3
		, hMemDC
		, 0, 0
		, 32, 48
		, RGB(255, 0, 255));
}

void CSelectFocus::Release()
{
}

// 키 입력 처리
// 포커스를 좌우로 이동하며 선택한 캐릭터 번호 교체
void CSelectFocus::Key_Input(int _Key)
{
	switch (_Key)
	{
	case VK_LEFT:
		if (m_eCurCharacter - 1 < 0)
			m_eCurCharacter = PLAYER::HEI_COB;
		else
			m_eCurCharacter = (PLAYER::NAME)(m_eCurCharacter - 1);

		m_tInfo.fX = 54.f + (32.f * 3 * m_eCurCharacter);
		break;
	case VK_RIGHT:
		if (m_eCurCharacter + 1 >= PLAYER::NAME_END)
			m_eCurCharacter = PLAYER::ALUCARD;
		else
			m_eCurCharacter = (PLAYER::NAME)(m_eCurCharacter + 1);

		m_tInfo.fX = 54.f + (32.f * 3 * m_eCurCharacter);
		break;
	}
}

// 현재 선택한 캐릭터 번호 얻기
PLAYER::NAME CSelectFocus::Get_SelectPlayer()
{
	return m_ePreCharacter;
}

// 캐릭터 교체
void CSelectFocus::CharacterChange()
{
	// 교체할 캐릭터가 있으면
	if (m_ePreCharacter != m_eCurCharacter)
	{
		switch (m_eCurCharacter)
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
		default:
			break;
		}
		// 교체할 캐릭터 저장
		m_ePreCharacter = m_eCurCharacter;
	}
}
