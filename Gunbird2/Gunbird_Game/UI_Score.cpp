// 기명준
// UI 점수
// 게임 화면에 점수를 숫자 이미지로 출력

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

// 업데이트
int CUI_Score::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CUI_Score::Late_Update()
{
}

// 렌더
void CUI_Score::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_SmallNumber");
	
	int iDivision = 10;		// 나누기용
	int iNum;				// 출력할 숫자 하나
	int iScore = m_iScore;	// 숫자를 나누고 남은 점수
	float fX = m_tInfo.fX;	// 출력할 숫자 좌표 X

	do {
		// 출력할 숫자
		iNum = iScore % iDivision;
		// 숫자 이미지 출력
		GdiTransparentBlt(_DC
			, fX, m_tInfo.fY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, iNum * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 숫자 출력할 X좌표 왼쪽으로 이동
		fX = fX - m_tInfo.iCX;
		// 자리수 버림
		iScore = iScore / iDivision;
	} while (iScore != 0);
}

void CUI_Score::Release()
{
}
