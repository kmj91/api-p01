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

// ������Ʈ
int CUI_Score::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CUI_Score::Late_Update()
{
}

// ����
void CUI_Score::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_SmallNumber");
	
	int iDivision = 10;		// �������
	int iNum;				// ����� ���� �ϳ�
	int iScore = m_iScore;	// ���ڸ� ������ ���� ����
	float fX = m_tInfo.fX;	// ����� ���� ��ǥ X

	do {
		// ����� ����
		iNum = iScore % iDivision;
		// ���� �̹��� ���
		GdiTransparentBlt(_DC
			, fX, m_tInfo.fY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, iNum * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// ���� ����� X��ǥ �������� �̵�
		fX = fX - m_tInfo.iCX;
		// �ڸ��� ����
		iScore = iScore / iDivision;
	} while (iScore != 0);
}

void CUI_Score::Release()
{
}
