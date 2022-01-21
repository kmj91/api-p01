// �����
// �� �Ѿ� 1
// ������ ���� ���� �����Ǽ� �߻��

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
	// �̹��� ��ġ �� ũ�� �ʱ�ȭ
	m_tInfo.iCX = BULLET_1_WIDTH * 3;
	m_tInfo.iCY = BULLET_1_HEIGHT * 3;
	m_tHitRectPos = { 3 * 3, 3 * 3, 9 * 3, 9 * 3 };
	m_iImageWidth = BULLET_1_WIDTH;
	m_iImageHeight = BULLET_1_HEIGHT;
	// �̹��� ������ �ʱ�ȭ
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 1;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;
	// �̵� �ӵ�
	m_fSpeed = 5.f;
}

// ������Ʈ
int CBullet_1::Update()
{
	// �Ѿ� ����
	if(m_bRemove)
		return OBJ_DEAD;

	// �Ѿ� ������ �ð��� ������ �÷��� true
	if (m_dwBulletTime + m_dwBulletDelay > GetTickCount())
		return OBJ_NOEVENT;
	else
		m_bOnBullet = true;

	// �Ѿ��� �ε����ų� ��ź���� ���ŵ�
	if (m_bDead) {
		// �Ѿ� �ı� ����Ʈ ����
		CObj* pObj = CObjFactory<CBulletDestroy>::Create(m_tInfo.fX, m_tInfo.fY, BULLETTYPE::BULLET_1);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
	// �Ѿ� �̵�
	else {
		m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
		m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;
	}
	
	// �̹��� ������ �̵�
	Frame_Move();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CBullet_1::Late_Update()
{
	// �÷��װ� false�� ����
	if (!m_bOnBullet)
		return;

	// �� ������Ʈ�� Update���� �����Ǽ� Update�� �ǳ� �ٰ� Late_Update�� �ϴ� ��찡 �߻���
	// �׷��� Late_Udpate���� ������
	// �̹��� RECT �� Hit RECT ���� ����
	Update_Rect();

	// �� �ٱ����� ������ ����
	if (-50 >= m_tRect.right || -50 >= m_tRect.bottom
		|| WINCX + 50 <= m_tRect.left || WINCY + 50 <= m_tRect.top)
		m_bDead = true;
}

void CBullet_1::Render(HDC _DC)
{
	// �÷��װ� false�� ����
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

	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CBullet_1::Release()
{
}
