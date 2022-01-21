// 기명준
// 폭탄 사용시 적 총알이 변환된 사탕 오브젝트
// 순간적으로 떠오르다가 맵 하단으로 쭉 떨어짐
// 충돌하면 점수 획득

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
	// 이미지 크기 및 Hit RECT 초기화
	m_tInfo.iCX = CANDY_WIDTH * 3;
	m_tInfo.iCY = CANDY_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, CANDY_WIDTH * 3, CANDY_HEIGHT * 3 };
	m_iImageWidth = CANDY_WIDTH;
	m_iImageHeight = CANDY_HEIGHT;
	// 여러 종류의 사탕 랜덤 선택
	m_tFrame.iFrameScene = rand() % 3;
	m_iDrawID = rand() % 22;
	// 랜덤 각도
	m_fAngle = (float)(90 + (rand() % 20 - 10));
	// 이동 속도
	m_fSpeed = 3.f;
	// 아이템 타입 초기화
	m_enItemType = ITEM::SCORE;
	// 점수
	m_iScore = 200;
}

// 업데이트
int CCandy::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead) {
		// 점수 텍스트 출력
		CObj* pObj = CObjFactory<CUI_ScoreText>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_SceneID(0);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);

		// 획득 사운드
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM);
		CSoundMgr::Get_Instance()->PlaySound(L"Item_Candy.wav", CSoundMgr::ITEM);

		return OBJ_DEAD;
	}

	// 10보다 작으면 지속적으로 증가
	if (m_fForce < 10.f)
		m_fForce += 0.05f;

	// 이동
	// 속도 값이 +부호에서 -로 변하면서 점점 빠르게 맵 하단으로 떨어짐
	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * (m_fSpeed - m_fForce);
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * (m_fSpeed - m_fForce);

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CCandy::Late_Update()
{
	// 이 오브젝트가 Update에서 생성되서 Update를 건너 뛰고 Late_Update를 하는 경우가 발생함
	// 그래서 Late_Udpate에서 갱신함
	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();

	// 맵 바깥으로 나가면 삭제
	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
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

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CCandy::Release()
{
}
