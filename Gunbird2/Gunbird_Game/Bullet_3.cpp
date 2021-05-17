#include "stdafx.h"
#include "Bullet_3.h"

#include "BmpMgr.h"
#include "ObjMgr.h"]

#include "BulletDestroy.h"


CBullet_3::CBullet_3()
{
}


CBullet_3::~CBullet_3()
{
	Release();
}

void CBullet_3::Initialize()
{
	m_tInfo.iCX = BULLET_3_WIDTH * 3;
	m_tInfo.iCY = BULLET_3_HEIGHT * 3;
	m_tHitRectPos = { 4 * 3, 4 * 3, 9 * 3, 9 * 3 };
	m_iImageWidth = BULLET_3_WIDTH;
	m_iImageHeight = BULLET_3_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;

	m_fSpeed = 8.f;
}

int CBullet_3::Update()
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
		CObj* pObj = CAbstractFactory<CBulletDestroy>::Create(m_tInfo.fX, m_tInfo.fY, BULLETTYPE::BULLET_3);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
	// ÃÑ¾Ë ÀÌµ¿
	else {
		m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
		m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;
	}

	// º¸Á¤
	if (180 < m_fAngle) {
		m_fAngle = m_fAngle - 360;
	}
	else if (m_fAngle < -180) {
		m_fAngle = m_fAngle + 360;
	}

	if (-5.f < m_fAngle && m_fAngle < 5.f)
		m_iDrawID = CBullet_3::A0;
	else if (-15.f <= m_fAngle && m_fAngle <= -5.f)
		m_iDrawID = CBullet_3::A_10;
	else if (-25.f <= m_fAngle && m_fAngle < -15.f)
		m_iDrawID = CBullet_3::A_20;
	else if (-35.f <= m_fAngle && m_fAngle < -25.f)
		m_iDrawID = CBullet_3::A_30;
	else if (-55.f <= m_fAngle && m_fAngle < -35.f)
		m_iDrawID = CBullet_3::A_45;
	else if (-70.f <= m_fAngle && m_fAngle < -55.f)
		m_iDrawID = CBullet_3::A_60;
	else if (-85.f <= m_fAngle && m_fAngle < -70.f)
		m_iDrawID = CBullet_3::A_75;
	else if (-95.f <= m_fAngle && m_fAngle < -85.f)
		m_iDrawID = CBullet_3::A_90;
	else if (-110.f <= m_fAngle && m_fAngle < -95.f)
		m_iDrawID = CBullet_3::A_105;
	else if (-125.f <= m_fAngle && m_fAngle < -110.f)
		m_iDrawID = CBullet_3::A_120;
	else if (-140.f <= m_fAngle && m_fAngle < -125.f)
		m_iDrawID = CBullet_3::A_135;
	else if (-155.f <= m_fAngle && m_fAngle < -140.f)
		m_iDrawID = CBullet_3::A_150;
	else if (-165.f <= m_fAngle && m_fAngle < -155.f)
		m_iDrawID = CBullet_3::A_160;
	else if (-175.f <= m_fAngle && m_fAngle < -165.f)
		m_iDrawID = CBullet_3::A_170;
	else if (-175.f > m_fAngle || 175.f <= m_fAngle)
		m_iDrawID = CBullet_3::A180;
	else if (165.f <= m_fAngle && m_fAngle < 175.f)
		m_iDrawID = CBullet_3::A170;
	else if (155.f <= m_fAngle && m_fAngle < 165.f)
		m_iDrawID = CBullet_3::A160;
	else if (145.f <= m_fAngle && m_fAngle < 155.f)
		m_iDrawID = CBullet_3::A150;
	else if (130.f <= m_fAngle && m_fAngle < 145.f)
		m_iDrawID = CBullet_3::A135;
	else if (115.f <= m_fAngle && m_fAngle < 130.f)
		m_iDrawID = CBullet_3::A120;
	else if (100.f <= m_fAngle && m_fAngle < 115.f)
		m_iDrawID = CBullet_3::A105;
	else if (85.f <= m_fAngle && m_fAngle < 100.f)
		m_iDrawID = CBullet_3::A90;
	else if (70.f <= m_fAngle && m_fAngle < 85.f)
		m_iDrawID = CBullet_3::A75;
	else if (55.f <= m_fAngle && m_fAngle < 70.f)
		m_iDrawID = CBullet_3::A60;
	else if (40.f <= m_fAngle && m_fAngle < 55.f)
		m_iDrawID = CBullet_3::A45;
	else if (25.f <= m_fAngle && m_fAngle < 40.f)
		m_iDrawID = CBullet_3::A30;
	else if (15.f <= m_fAngle && m_fAngle < 25.f)
		m_iDrawID = CBullet_3::A20;
	else if (5.f <= m_fAngle && m_fAngle < 15.f)
		m_iDrawID = CBullet_3::A10;


	Frame_Move();

	return OBJ_NOEVENT;
}

void CBullet_3::Late_Update()
{
	if (!m_bOnBullet)
		return;

	Update_Rect();

	if (-50 >= m_tRect.right || -50 >= m_tRect.bottom
		|| WINCX + 50 <= m_tRect.left || WINCY + 50 <= m_tRect.top)
		m_bDead = true;
}

void CBullet_3::Render(HDC _DC)
{
	if (!m_bOnBullet)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet_3");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_iDrawID * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// Ãæµ¹ ¹Ú½º
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CBullet_3::Release()
{
}
