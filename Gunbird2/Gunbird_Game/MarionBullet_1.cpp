// 기명준
// 마리온 기본 총알

#include "stdafx.h"
#include "MarionBullet_1.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "BulletHit.h"


CMarionBullet_1::CMarionBullet_1()
{
}


CMarionBullet_1::~CMarionBullet_1()
{
	Release();
}

void CMarionBullet_1::Initialize()
{
	// 이미지 위치 및 크기 초기화
	m_tInfo.iCX = MARION_BULLET_POWER_1_WIDTH * 3;
	m_tInfo.iCY = MARION_BULLET_POWER_1_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, MARION_BULLET_POWER_1_WIDTH * 3, MARION_BULLET_POWER_1_HEIGHT * 3 };
	m_iImageWidth = MARION_BULLET_POWER_1_WIDTH;
	m_iImageHeight = MARION_BULLET_POWER_1_HEIGHT;
	// 이동 속도
	m_fSpeed = 20.f;
	// 데미지
	m_fDamage = 8.f;
}

// 업데이트
int CMarionBullet_1::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 명중
	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CBulletHit>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
	// 이동
	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CMarionBullet_1::Late_Update()
{
	// 이 오브젝트가 Update에서 생성되서 Update를 건너 뛰고 Late_Update를 하는 경우가 발생함
	// 그래서 Late_Udpate에서 갱신함
	// 이미지 RECT 및 Hit RECT 정보 갱신
	Update_Rect();

	// 맵 바깥으로 나가면 삭제
	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
void CMarionBullet_1::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Bullet_1");

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
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CMarionBullet_1::Release()
{
}
