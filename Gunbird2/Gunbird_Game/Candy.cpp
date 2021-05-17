#include "stdafx.h"
#include "Candy.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "UI_ScoreText.h"


CCandy::CCandy() : m_fForce(0.f)
{
}


CCandy::~CCandy()
{
	Release();
}

void CCandy::Initialize()
{
	m_tInfo.iCX = CANDY_WIDTH * 3;
	m_tInfo.iCY = CANDY_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, CANDY_WIDTH * 3, CANDY_HEIGHT * 3 };
	m_iImageWidth = CANDY_WIDTH;
	m_iImageHeight = CANDY_HEIGHT;

	m_tFrame.iFrameScene = rand() % 3;
	m_iDrawID = rand() % 22;
	m_fAngle = (float)(90 + (rand() % 20 - 10));
	m_fSpeed = 3.f;

	m_enItemType = ITEM::SCORE;
	m_iScore = 200;
}

int CCandy::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead) {
		// 점수 텍스트 출력
		CObj* pObj = CAbstractFactory<CUI_ScoreText>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_SceneID(0);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);

		// 획득 사운드
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM);
		CSoundMgr::Get_Instance()->PlaySound(L"Item_Candy.wav", CSoundMgr::ITEM);

		return OBJ_DEAD;
	}

	if (m_fForce < 10.f)
		m_fForce += 0.05f;

	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * (m_fSpeed - m_fForce);
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * (m_fSpeed - m_fForce);

	return OBJ_NOEVENT;
}

void CCandy::Late_Update()
{
	Update_Rect();

	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

void CCandy::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Candy");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 충돌 박스
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CCandy::Release()
{
}
