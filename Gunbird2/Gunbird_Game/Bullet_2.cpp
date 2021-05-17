#include "stdafx.h"
#include "Bullet_2.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "BulletDestroy.h"


CBullet_2::CBullet_2()
{
}


CBullet_2::~CBullet_2()
{
	Release();
}

void CBullet_2::Initialize()
{
	m_tInfo.iCX = BULLET_2_WIDTH * 3;
	m_tInfo.iCY = BULLET_2_HEIGHT * 3;
	m_tHitRectPos = { 4 * 4, 3 * 3, 12 * 3, 12 * 3 };
	m_iImageWidth = BULLET_2_WIDTH;
	m_iImageHeight = BULLET_2_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 1;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;

	m_fSpeed = 8.f;
}

int CBullet_2::Update()
{
	// ÃÑ¾Ë »èÁ¦
	if (m_bRemove)
		return OBJ_DEAD;

	// ÃÑ¾Ë µô·¹ÀÌ
	if (m_dwBulletTime + m_dwBulletDelay > GetTickCount())
		return OBJ_NOEVENT;
	else
		m_bOnBullet = true;

	// ÃÑ¾ËÀÌ ºÎµúÈ÷°Å³ª ÆøÅºÀ¸·Î Á¦°ÅµÊ
	if (m_bDead) {
		CObj* pObj = CAbstractFactory<CBulletDestroy>::Create(m_tInfo.fX, m_tInfo.fY, BULLETTYPE::BULLET_2);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
	// ÃÑ¾Ë ÀÌµ¿
	else {
		m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
		m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;
	}

	Frame_Move();

	return OBJ_NOEVENT;
}

void CBullet_2::Late_Update()
{
	if (!m_bOnBullet)
		return;

	Update_Rect();

	if (-50 >= m_tRect.right || -50 >= m_tRect.bottom
		|| WINCX + 50 <= m_tRect.left || WINCY + 50 <= m_tRect.top)
		m_bDead = true;
}

void CBullet_2::Render(HDC _DC)
{
	if (!m_bOnBullet)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet_2");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// Ãæµ¹ ¹Ú½º
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CBullet_2::Release()
{
}
