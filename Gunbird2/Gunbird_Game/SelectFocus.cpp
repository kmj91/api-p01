// �����
// ĳ���� ���� ǥ�� ������Ʈ
// ���� ��Ŀ�� ���� �� ȭ�鿡�� ������ ĳ���� ǥ��

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
	// �⺻ ��Ŀ�� ��ǥ
	m_tInfo.fX = 150;
	m_tInfo.fY = 760;
	m_tInfo.iCX = 32;
	m_tInfo.iCY = 48;

	// �⺻ ���� ĳ����
	m_eCurCharacter = PLAYER::MARION;
}

int CSelectFocus::Update()
{
	// ��Ʈ ������Ʈ
	Update_Rect();
	// ĳ���� ��ü
	CharacterChange();
	// ������ ó��
	Frame_Move();

	return OBJ_NOEVENT;
}

void CSelectFocus::Late_Update()
{
}

void CSelectFocus::Render(HDC _DC)
{
	// ������ ĳ���� ȭ�鿡 ǥ��
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"CharacterSelect");
	GdiTransparentBlt(_DC
		, 0, 0
		, 224 * 3, 320 * 3
		, hMemDC
		, m_tFrame.iFrameCnt * 224, m_tFrame.iFrameScene * 320
		, 224, 320
		, RGB(255, 0, 255));

	// ĳ���� ���� ��Ŀ��
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

// Ű �Է� ó��
// ��Ŀ���� �¿�� �̵��ϸ� ������ ĳ���� ��ȣ ��ü
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

// ���� ������ ĳ���� ��ȣ ���
PLAYER::NAME CSelectFocus::Get_SelectPlayer()
{
	return m_ePreCharacter;
}

// ĳ���� ��ü
void CSelectFocus::CharacterChange()
{
	// ��ü�� ĳ���Ͱ� ������
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
		// ��ü�� ĳ���� ����
		m_ePreCharacter = m_eCurCharacter;
	}
}
