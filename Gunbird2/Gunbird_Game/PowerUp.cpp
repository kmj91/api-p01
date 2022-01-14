// 기명준
// 파워업 아이템
// 화면 안에서 특정 각도로 부딪히며 돌아다님
// 파괴자 호출되면 파워업 텍스트 오브젝트 생성

#include "stdafx.h"
#include "PowerUp.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "UI_PowerUpDownText.h"


CPowerUp::CPowerUp() : m_fRadian(0.f)
{
}


CPowerUp::~CPowerUp()
{
	Release();
}

void CPowerUp::Initialize()
{
	// 이미지 크기 및 Hit RECT 초기화
	m_tInfo.iCX = POWER_UP_WIDTH * 3;
	m_tInfo.iCY = POWER_UP_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, POWER_UP_WIDTH * 3, POWER_UP_HEIGHT * 3 };
	m_iImageWidth = POWER_UP_WIDTH;
	m_iImageHeight = POWER_UP_HEIGHT;
	// 이미지 프레임 초기화
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;
	// 이동 속도
	m_fSpeed = 4.0f;
	// 랜덤 각도 초기화
	int iRadValue = rand() % 20;
	m_fAngle = -70.f + (float)iRadValue;
	m_fRadian = PI * m_fAngle / 180.f;
	// 아이템 타입 초기화
	m_enItemType = ITEM::POWER_UP;
}

// 업데이트
int CPowerUp::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += cosf(m_fRadian) * m_fSpeed;
	m_tInfo.fY += -sinf(m_fRadian) * m_fSpeed;

	//---------------------------
	// 화면 밖을 나가지 않는다
	//---------------------------
	if (m_tInfo.fX < 0) {
		m_fRadian = PI - m_fRadian;
	}
	if (m_tInfo.fX > WINCX) {
		m_fRadian = PI - m_fRadian;
	}
	if (m_tInfo.fY < 0) {
		m_fRadian = 2 * PI - m_fRadian;
	}
	if (m_tInfo.fY > WINCY - 200) {
		m_fRadian = 2 * PI - m_fRadian;
	}

	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
	// 이미지 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CPowerUp::Late_Update()
{
}

// 렌더
void CPowerUp::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PowerUp");
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

void CPowerUp::Release()
{
	// 파워 업 텍스트 출력
	CObj* pObj = CAbstractFactory<CUI_PowerUpDownText>::Create(m_tInfo.fX, m_tInfo.fY);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);
}
