// �����
// UI ũ����

#include "stdafx.h"
#include "UI_Credit.h"

#include "BmpMgr.h"


CUI_Credit::CUI_Credit() : m_iCredit(0)
{
}


CUI_Credit::~CUI_Credit()
{
	Release();
}

void CUI_Credit::Initialize()
{
	m_tInfo.iCX = UI_SMALL_NUMBER_WIDTH * 3;
	m_tInfo.iCY = UI_SMALL_NUMBER_HEIGHT * 3;
	m_iImageWidth = UI_SMALL_NUMBER_WIDTH;
	m_iImageHeight = UI_SMALL_NUMBER_HEIGHT;
}

int CUI_Credit::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CUI_Credit::Late_Update()
{
}

void CUI_Credit::Render(HDC _DC)
{
	// �ؽ�Ʈ
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Credit");
	GdiTransparentBlt(_DC
		, m_tInfo.fX - (UI_CREDIT_TEXT_WIDTH * 3 / 1.3f), m_tInfo.fY + (UI_CREDIT_TEXT_HEIGHT * 3 / 2)
		, UI_CREDIT_TEXT_WIDTH * 3, UI_CREDIT_TEXT_HEIGHT * 3
		, hMemDC
		, 0, 0
		, UI_CREDIT_TEXT_WIDTH, UI_CREDIT_TEXT_HEIGHT
		, RGB(255, 0, 255));


	// ����
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_SmallNumber");
	
	int iCnt = 10;	// �������
	int iNum;		// ����� ���� �ϳ�
	int iScore = m_iCredit;	// ���� ��
	float fX = m_tInfo.fX + m_tInfo.iCX * 2;
	float fY = m_tInfo.fY;

	do {
		// ����� ����
		iNum = iScore % iCnt;

		GdiTransparentBlt(_DC
			, fX, fY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, iNum * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// ���� ����� X��ǥ �������� �̵�
		fX = fX - m_tInfo.iCX;
		// �ڸ��� ����
		iScore = iScore / iCnt;
	} while (iScore != 0);

	
}

void CUI_Credit::Release()
{
}
