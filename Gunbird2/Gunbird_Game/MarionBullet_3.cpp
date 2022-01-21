// �����
// ������ �Ѿ� �Ŀ� ���� 2

#include "stdafx.h"
#include "MarionBullet_3.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "BulletHit.h"


CMarionBullet_3::CMarionBullet_3()
{
}


CMarionBullet_3::~CMarionBullet_3()
{
	Release();
}

void CMarionBullet_3::Initialize()
{
	// �̹��� ��ġ �� ũ�� �ʱ�ȭ
	m_tInfo.iCX = MARION_BULLET_POWER_3_WIDTH * 3;
	m_tInfo.iCY = MARION_BULLET_POWER_3_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, MARION_BULLET_POWER_3_WIDTH * 3, MARION_BULLET_POWER_3_HEIGHT * 3 };
	m_iImageWidth = MARION_BULLET_POWER_3_WIDTH;
	m_iImageHeight = MARION_BULLET_POWER_3_HEIGHT;
	// �̵� �ӵ�
	m_fSpeed = 25.f;
	// ������
	m_fDamage = 8.f;
}

// ������Ʈ
int CMarionBullet_3::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;
	// ����
	if (m_bDead)
	{
		CObj* pObj = CObjFactory<CBulletHit>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
	// �̵�
	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CMarionBullet_3::Late_Update()
{
	// �� ������Ʈ�� Update���� �����Ǽ� Update�� �ǳ� �ٰ� Late_Update�� �ϴ� ��찡 �߻���
	// �׷��� Late_Udpate���� ������
	// �̹��� RECT �� Hit RECT ���� ����
	Update_Rect();

	// �� �ٱ����� ������ ����
	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
void CMarionBullet_3::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Bullet_3");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CMarionBullet_3::Release()
{
}
