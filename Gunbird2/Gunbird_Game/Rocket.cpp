// 기명준
// 적 몬스터 로켓
// 별다른 공격 능력은 없고 아래쪽을 향해 이동

#include "stdafx.h"
#include "Rocket.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "Explosion_01.h"


CRocket::CRocket()
{
}


CRocket::~CRocket()
{
	Release();
}

void CRocket::Initialize()
{
	// 기본 위치 및 크기 설정
	m_tInfo.iCX = ROCKET_WIDTH * 3;
	m_tInfo.iCY = ROCKET_HEIGHT * 3;
	m_tHitRectPos = { 1 * 3, 5 * 3, 20 * 3, 30 * 3 };
	m_iImageWidth = ROCKET_WIDTH;
	m_iImageHeight = ROCKET_HEIGHT;
	// 기본 애니메이션 프레임 설정
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 20;
	// 이동 속도
	m_fSpeed = 6.0f;

	// 점수
	m_iScore = 200;
}

// 업데이트
int CRocket::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}

	m_tInfo.fY += m_fSpeed;

	Update_Rect();
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CRocket::Late_Update()
{
	if (m_bHpLock)
		if (0 <= m_tRect.top)
			m_bHpLock = false;

	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
void CRocket::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Rocket");

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
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CRocket::Release()
{
}
