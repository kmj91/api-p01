// 기명준
// 적이 파괴된 후 드랍하는 코인 아이템
// 맵 스크롤과 같은 속도로 이동 함 (지상 오브젝트 취급)
// 충돌하면 점수 획득

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
	// 이미지 크기 및 Hit RECT 초기화
	m_tInfo.iCX = COIN_WIDTH * 3;
	m_tInfo.iCY = COIN_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, COIN_WIDTH * 3, COIN_HEIGHT * 3 };
	m_iImageWidth = COIN_WIDTH;
	m_iImageHeight = COIN_HEIGHT;
	// 이미지 프레임 초기화
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;
	// 아이템 타입 초기화
	m_enItemType = ITEM::SCORE;
	// 점수
	m_iScore = 500;
}

// 업데이트
int CCoin::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;

	// 플레이어와 충돌해서 획득된 상황
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

	// 배경에 맞춰서 Y축 이동
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;
	// 이미지 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CCoin::Late_Update()
{
	// 이 오브젝트가 Update에서 생성되서 Update를 건너 뛰고 Late_Update를 하는 경우가 발생함
	// 그래서 Late_Udpate에서 갱신함
	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();

	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
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

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CCoin::Release()
{
}
