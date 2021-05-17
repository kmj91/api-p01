#include "stdafx.h"
#include "Coin.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "UI_ScoreText.h"


CCoin::CCoin()
{
}


CCoin::~CCoin()
{
	Release();
}

void CCoin::Initialize()
{
	m_tInfo.iCX = COIN_WIDTH * 3;
	m_tInfo.iCY = COIN_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, COIN_WIDTH * 3, COIN_HEIGHT * 3 };
	m_iImageWidth = COIN_WIDTH;
	m_iImageHeight = COIN_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;

	m_enItemType = ITEM::SCORE;
	m_iScore = 500;
}

int CCoin::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead) {
		// 점수 텍스트 출력
		CObj* pObj = CAbstractFactory<CUI_ScoreText>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_SceneID(1);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);

		// 획득 사운드
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM);
		CSoundMgr::Get_Instance()->PlaySound(L"Item_Coin.wav", CSoundMgr::ITEM);

		return OBJ_DEAD;
	}

	m_tInfo.fY += g_fBackgroundSpeed * 3.f;			// 배경에 맞춰서 Y축 이동

	Frame_Move();

	return OBJ_NOEVENT;
}

void CCoin::Late_Update()
{
	Update_Rect();

	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

void CCoin::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Coin");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 충돌 박스
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CCoin::Release()
{
}
