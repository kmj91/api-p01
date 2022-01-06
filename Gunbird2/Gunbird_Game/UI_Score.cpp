// �����
// UI ����
// ���� ȭ�鿡 ������ ���� �̹����� ���

#include "stdafx.h"
#include "UI_Score.h"

#include "BmpMgr.h"


CUI_Score::CUI_Score() : m_iScore(0)
{
}


CUI_Score::~CUI_Score()
{
	Release();
}

void CUI_Score::Initialize()
{
	m_tInfo.iCX = UI_SMALL_NUMBER_WIDTH * 3;
	m_tInfo.iCY = UI_SMALL_NUMBER_HEIGHT * 3;
	m_iImageWidth = UI_SMALL_NUMBER_WIDTH;
	m_iImageHeight = UI_SMALL_NUMBER_HEIGHT;
}

int CUI_Score::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CUI_Score::Late_Update()
{
}

void CUI_Score::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_SmallNumber");
	
	int iCnt = 10;	// �������
	int iNum;		// ����� ���� �ϳ�
	int iScore = m_iScore;	// ���� ��
	float fX = m_tInfo.fX;
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

void CUI_Score::Release()
{
}
