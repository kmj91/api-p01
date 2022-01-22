#include "stdafx.h"
#include "MarionBombEffect.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "MarionEffect.h"


CMarionBombEffect::CMarionBombEffect()
	: m_fDestX(0.f), m_fDestY(0.f), m_dwEffectCreateTime(0), m_dwEffectCreateDelay(100)
{
}


CMarionBombEffect::~CMarionBombEffect()
{
	Release();
}

void CMarionBombEffect::Initialize()
{
	// �̹��� ũ�� �ʱ�ȭ
	m_tInfo.iCX = MARION_GUIDED_BULLET_WIDTH * 3;
	m_tInfo.iCY = MARION_GUIDED_BULLET_HEIGHT * 3;
	m_iImageWidth = MARION_GUIDED_BULLET_WIDTH;
	m_iImageHeight = MARION_GUIDED_BULLET_HEIGHT;
	// �̵� �ӵ�
	m_fSpeed = 10.f;
}

// ������Ʈ
int CMarionBombEffect::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;

	// ����Ʈ ������ �ð��� ������ �÷��� true
	if (!m_bOnEffect && m_dwEffectTime + m_dwEffectDelay > GetTickCount())
		return OBJ_NOEVENT;
	else
		m_bOnEffect = true;

	// ȸ�� �̵�
	if (Move()) {
		// �����ϸ� ����
		m_bRemove = true;
	}
	// ������ ���� �̹��� ����
	Angle();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CMarionBombEffect::Late_Update()
{
	// �� ������Ʈ�� Update���� �����Ǽ� Update�� �ǳ� �ٰ� Late_Update�� �ϴ� ��찡 �߻���
	// �׷��� Late_Udpate���� ������
	// �̹��� RECT ���� ����
	Update_Rect();

	// �� �ٱ����� ������ ����
	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
void CMarionBombEffect::Render(HDC _DC)
{
	// �÷��װ� true�� �ƴϸ� ������ ���� ����
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Guided_Bullet");

	GdiTransparentBlt(_DC
		, m_tInfo.fX, m_tInfo.fY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CMarionBombEffect::Release()
{
}

// ȸ�� �̵�
bool CMarionBombEffect::Move()
{
	float fX = m_fDestX - m_tInfo.fX;		// ���� ��ǥ X
	float fY = m_fDestY - m_tInfo.fY;		// ���� ��ǥ Y
	float fDia = sqrtf(fX * fX + fY * fY);	// ���� ��ġ���� ���������� �Ÿ�
	float fvx;					// ���� �̵� ��ǥ X
	float fvy;					// ���� �̵� ��ǥ Y
	float fRad;					// _fAngle ���� ��
	float fvx2;					// ȸ�� ��� ��ǥ �� X
	float fvy2;					// ȸ�� ��� ��ǥ �� Y
	float fSpeed = m_fSpeed;	// �̵� �ӵ�

	// ������������ �Ÿ��� �̵� �ӵ����� �۴ٸ�
	// ��ǥ �Ÿ���ŭ �̵��� �� �ְ� �ӵ� ���� �ٲ�
	if (fDia < fSpeed)
		fSpeed = fDia;

	// fX / fDia ����� �̵� �Ÿ��� 1�� �� �̵��ϴ� X ��ǥ �� ��������
	// �������Ϳ� fSpeed(�̵� �ӵ�)�� ���ؼ� ���ϴ� ��ŭ �̵��ϴ� ��ǥ ���� ����
	// fvx, fvy�� �����̵����� ���� ��ǥ ��
	fvx = fX / fDia * fSpeed;
	fvy = fY / fDia * fSpeed;

	// ��ȸ ���� ���� ��
	fRad = PI / 180.f * m_fAngle;
	// ȸ�� �̵��� ��ǥ ��
	fvx2 = cos(fRad) * fvx - sin(fRad) * fvy;
	fvy2 = sin(fRad) * fvx + cos(fRad) * fvy;
	// ������ ���� ���� ���� ��ǥ�� ���ؼ� ��ǥ �̵�
	m_tInfo.fX += fvx2;
	m_tInfo.fY += fvy2;

	// ����Ʈ ����
	if (m_dwEffectCreateTime + m_dwEffectCreateDelay < GetTickCount())
	{
		m_dwEffectCreateTime = GetTickCount();
		CObj* pObj = CObjFactory<CMarionEffect>::Create(m_tInfo.fX, m_tRect.bottom);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
	}

	// �������� �����ߴ���?
	if (fDia - fSpeed == 0)
		return true;

	return false;
}

// ������ ���� �̹��� ����
void CMarionBombEffect::Angle()
{
	float fX = m_fDestX - m_tInfo.fX;		// ���� ��ǥ X
	float fY = m_fDestY - m_tInfo.fY;		// ���� ��ǥ Y
	float fDia = sqrtf(fX * fX + fY * fY);	// �Ÿ�
	// arccos(cos) = ���� ���� ����
	float fRad = acosf(fX / fDia);
	// ���� ������ ������ ����
	float fAngle = fRad * 180.f / PI;
	// �������� Y ��ǥ�� ���� Y ��ǥ ���� ũ�� ��ȣ ����
	if (m_tInfo.fY < m_fDestY)
		fAngle *= -1.f;

	// ����
	if (180 < fAngle) {
		fAngle = fAngle - 360;
	}
	else if (fAngle < -180) {
		fAngle = fAngle + 360;
	}

	// ������ ���� �̹��� �ε��� ��ȣ ��ü
	if (80.f < fAngle && fAngle < 100.f)
		m_iDrawID = CMarionBombEffect::A90;
	else if (60.f <= fAngle && fAngle <= 80.f)
		m_iDrawID = CMarionBombEffect::A70;
	else if (30.f <= fAngle && fAngle < 60.f)
		m_iDrawID = CMarionBombEffect::A45;
	else if (10.f <= fAngle && fAngle < 30.f)
		m_iDrawID = CMarionBombEffect::A20;
	else if (-10.f < fAngle && fAngle < 10.f)
		m_iDrawID = CMarionBombEffect::A0;
	else if (-30.f <= fAngle && fAngle <= -10.f)
		m_iDrawID = CMarionBombEffect::A_20;
	else if (-60.f <= fAngle && fAngle < -30.f)
		m_iDrawID = CMarionBombEffect::A_45;
	else if (-80.f <= fAngle && fAngle < -60.f)
		m_iDrawID = CMarionBombEffect::A_70;
	else if (-100.f < fAngle && fAngle < -80.f)
		m_iDrawID = CMarionBombEffect::A_90;
	else if (-120.f <= fAngle && fAngle <= -100.f)
		m_iDrawID = CMarionBombEffect::A_110;
	else if (-150.f <= fAngle && fAngle < -120.f)
		m_iDrawID = CMarionBombEffect::A_135;
	else if (-170.f <= fAngle && fAngle < -150.f)
		m_iDrawID = CMarionBombEffect::A_160;
	else if (170.f < fAngle && fAngle < -170.f)
		m_iDrawID = CMarionBombEffect::A180;
	else if (150.f <= fAngle && fAngle <= 170.f)
		m_iDrawID = CMarionBombEffect::A160;
	else if (120.f <= fAngle && fAngle < 150.f)
		m_iDrawID = CMarionBombEffect::A135;
	else if (100.f <= fAngle && fAngle < 120.f)
		m_iDrawID = CMarionBombEffect::A110;
}
