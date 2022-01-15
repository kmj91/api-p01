// �����
// ������ ���� �̻���

#include "stdafx.h"
#include "MarionGuidedBullet.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "BulletHit.h"
#include "MarionEffect.h"


CMarionGuidedBullet::CMarionGuidedBullet()
	: m_dwEffectCreateTime(0), m_dwEffectCreateDelay(100)
	, m_stpSubBulletCnt(nullptr)
{
}


CMarionGuidedBullet::~CMarionGuidedBullet()
{
	Release();
}

void CMarionGuidedBullet::Initialize()
{
	// �̹��� ��ġ �� ũ�� �ʱ�ȭ
	m_tInfo.iCX = MARION_GUIDED_BULLET_WIDTH * 3;
	m_tInfo.iCY = MARION_GUIDED_BULLET_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, MARION_GUIDED_BULLET_WIDTH * 3, MARION_GUIDED_BULLET_HEIGHT * 3 };
	m_iImageWidth = MARION_GUIDED_BULLET_WIDTH;
	m_iImageHeight = MARION_GUIDED_BULLET_HEIGHT;
	// �̵� �ӵ�
	m_fSpeed = 16.f;
	// ������
	m_fDamage = 2.f;
}

// ������Ʈ
int CMarionGuidedBullet::Update()
{
	// ����
	if (m_bRemove)
	{
		--m_stpSubBulletCnt->count;
		if (m_stpSubBulletCnt->count == 0)		// ȭ�鿡�� �Ѿ� ��ü�� ��� ������� ������ �� ����
		{
			m_stpSubBulletCnt->flag = false;
		}
		return OBJ_DEAD;
	}	
	// �浹
	if (m_bDead)
	{
		--m_stpSubBulletCnt->count;
		if (m_stpSubBulletCnt->count == 0)		// ȭ�鿡�� �Ѿ� ��ü�� ��� ������� ������ �� ����
		{
			m_stpSubBulletCnt->flag = false;
		}

		CObj* pObj = CAbstractFactory<CBulletHit>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
		
	// �̵�
	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	// ����Ʈ ����
	if (m_dwEffectCreateTime + m_dwEffectCreateDelay < GetTickCount())
	{
		m_dwEffectCreateTime = GetTickCount();
		CObj* pObj = CAbstractFactory<CMarionEffect>::Create(m_tInfo.fX, m_tRect.bottom);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
	}

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CMarionGuidedBullet::Late_Update()
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
void CMarionGuidedBullet::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Guided_Bullet");

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

void CMarionGuidedBullet::Release()
{
}

// ���� �ʱ�ȭ
// Ÿ�ٰ��� ������ ���ؼ� �̹��� �ε����� ��ü��
void CMarionGuidedBullet::Init_Angle()
{
	// Ÿ���� ������ �ش� Ÿ���� ��ǥ��
	// ������ ����Ʈ ������ ���ư�
	if (m_pTarget)
	{
		float fX = m_pTarget->Get_Info().fX - m_tInfo.fX;	// ���� ��ǥ X
		float fY = m_pTarget->Get_Info().fY - m_tInfo.fY;	// ���� ��ǥ Y
		float fDia = sqrtf(fX * fX + fY * fY);				// �Ÿ�
		// arccos(cos) = ���� ���� ����
		float fRad = acosf(fX / fDia);
		// ���� ������ ȣ������ ����
		m_fAngle = fRad * 180.f / PI;
		// Ÿ���� Y ��ǥ�� ���� Y ��ǥ ���� ũ�� ��ȣ ����
		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			m_fAngle *= -1.f;
	}

	// ������ �� �̹��� �ε����� ��ü
	if (80.f < m_fAngle && m_fAngle < 100.f)
		m_iDrawID = CMarionGuidedBullet::A90;
	else if (60.f <= m_fAngle && m_fAngle <= 80.f)
		m_iDrawID = CMarionGuidedBullet::A70;
	else if (30.f <= m_fAngle && m_fAngle < 60.f)
		m_iDrawID = CMarionGuidedBullet::A45;
	else if (10.f <= m_fAngle && m_fAngle < 30.f)
		m_iDrawID = CMarionGuidedBullet::A20;
	else if (-10.f < m_fAngle && m_fAngle < 10.f)
		m_iDrawID = CMarionGuidedBullet::A0;
	else if (-30.f <= m_fAngle && m_fAngle <= -10.f)
		m_iDrawID = CMarionGuidedBullet::A_20;
	else if (-60.f <= m_fAngle && m_fAngle < -30.f)
		m_iDrawID = CMarionGuidedBullet::A_45;
	else if (-80.f <= m_fAngle && m_fAngle < -60.f)
		m_iDrawID = CMarionGuidedBullet::A_70;
	else if (-100.f < m_fAngle && m_fAngle < -80.f)
		m_iDrawID = CMarionGuidedBullet::A_90;
	else if (-120.f <= m_fAngle && m_fAngle <= -100.f)
		m_iDrawID = CMarionGuidedBullet::A_110;
	else if (-150.f <= m_fAngle && m_fAngle < -120.f)
		m_iDrawID = CMarionGuidedBullet::A_135;
	else if (-170.f <= m_fAngle && m_fAngle < -150.f)
		m_iDrawID = CMarionGuidedBullet::A_160;
	else if (170.f < m_fAngle && m_fAngle < -170.f)
		m_iDrawID = CMarionGuidedBullet::A180;
	else if (150.f <= m_fAngle && m_fAngle <= 170.f)
		m_iDrawID = CMarionGuidedBullet::A160;
	else if (120.f <= m_fAngle && m_fAngle < 150.f)
		m_iDrawID = CMarionGuidedBullet::A135;
	else if (100.f <= m_fAngle && m_fAngle < 120.f)
		m_iDrawID = CMarionGuidedBullet::A110;
}
