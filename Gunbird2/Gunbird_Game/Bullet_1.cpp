// 기명준
// 적 총알 1
// 딜레이 값에 따라 지연되서 발사됨

#include "stdafx.h"
#include "Bullet_1.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "BulletDestroy.h"


CBullet_1::CBullet_1()
{
}


CBullet_1::~CBullet_1()
{
	Release();
}

void CBullet_1::Initialize()
{
	// 이미지 위치 및 크기 초기화
	m_tInfo.iCX = BULLET_1_WIDTH * 3;
	m_tInfo.iCY = BULLET_1_HEIGHT * 3;
	m_tHitRectPos = { 3 * 3, 3 * 3, 9 * 3, 9 * 3 };
	m_iImageWidth = BULLET_1_WIDTH;
	m_iImageHeight = BULLET_1_HEIGHT;
	// 이미지 프레임 초기화
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 1;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;
	// 이동 속도
	m_fSpeed = 5.f;
}

// 업데이트
int CBullet_1::Update()
{
	// 총알 삭제
	if(m_bRemove)
		return OBJ_DEAD;

	// 총알 딜레이 시간이 지나면 플래그 true
	if (m_dwBulletTime + m_dwBulletDelay > GetTickCount())
		return OBJ_NOEVENT;
	else
		m_bOnBullet = true;

	// 총알이 부딪히거나 폭탄으로 제거됨
	if (m_bDead) {
		// 총알 파괴 이펙트 생성
		CObj* pObj = CObjFactory<CBulletDestroy>::Create(m_tInfo.fX, m_tInfo.fY, BULLETTYPE::BULLET_1);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
	// 총알 이동
	else {
		m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
		m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;
	}
	
	// 이미지 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CBullet_1::Late_Update()
{
	// 플래그가 false면 예외
	if (!m_bOnBullet)
		return;

	// 이 오브젝트가 Update에서 생성되서 Update를 건너 뛰고 Late_Update를 하는 경우가 발생함
	// 그래서 Late_Udpate에서 갱신함
	// 이미지 RECT 및 Hit RECT 정보 갱신
	Update_Rect();

	// 맵 바깥으로 나가면 삭제
	if (-50 >= m_tRect.right || -50 >= m_tRect.bottom
		|| WINCX + 50 <= m_tRect.left || WINCY + 50 <= m_tRect.top)
		m_bDead = true;
}

void CBullet_1::Render(HDC _DC)
{
	// 플래그가 false면 예외
	if (!m_bOnBullet)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet_1");

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

void CBullet_1::Release()
{
}
