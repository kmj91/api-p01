// �����
// �� ���� �Ķ��� �︮����
// ���� ���� ��ȸ�ϸ鼭 ���� �׸��� ���� �ð� �� ����

#include "stdafx.h"
#include "Blue.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_01.h"
#include "Bullet_1.h"


CBlue::CBlue()
	: m_ePreState(CBlue::END), m_eCurState(CBlue::END), m_dwOnTime(0), m_dwOutTime(0),
	m_fDestPosX(0.f), m_fDestPosY(0.f), m_fOutPosX(0.f), m_fOutPosY(0.f), m_fDestAngle(0.f), m_fOutAngle(0.f),
	m_iShot(0), m_bOnObject(false), m_bOutObject(false)
{
}


CBlue::~CBlue()
{
	Release();
}

void CBlue::Initialize()
{
	// �⺻ ��ġ �� ����
	m_tInfo.iCX = BLUE_WIDTH * 3;
	m_tInfo.iCY = BLUE_HEIGHT * 3;
	m_tHitRectPos = { 10 * 3, 11 * 3, 32 * 3, 31 * 3 };
	m_iImageWidth = BLUE_WIDTH;
	m_iImageHeight = BLUE_HEIGHT;
	// �⺻ �ִϸ��̼� ������ ����
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 40;
	// �̵� �ӵ�
	m_fSpeed = 7.0f;
	// �⺻ ����
	m_eCurState = CBlue::FRONT;

	// ����
	m_iScore = 200;
}

int CBlue::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;
	// ����
	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}

	// ��ġ �Ǳ� ��
	if (!m_bOnObject)
	{
		// ��ġ ��ǥ�� �̵�
		if(Move(m_fDestPosX, m_fDestPosY, m_fDestAngle))
		{
			// �������� ����
			m_eCurState = CBlue::FRONT;
			m_bOnObject = true;
			m_dwOnTime = GetTickCount();
		}
	}
	// ��ġ �� ��
	else
	{
		// ���� �ϱ� ��
		if (!m_bOutObject)
		{
			// ���� �ð��ΰ�?
			DWORD dwTime = GetTickCount();
			if ((dwTime - m_dwOnTime) > m_dwOutTime) {
				m_bOutObject = true;
				m_iShot = 1;
			}
		}
		// ����
		else
		{
			// ���� ��ǥ�� �̵�
			if (Move(m_fOutPosX, m_fOutPosY, m_fOutAngle))
			{
				// ���� �÷��� true
				m_bRemove = true;
			}
		}
	}

	// �Ѿ��� ����������
	if (m_iShot > 0)
	{
		// �Ѿ� ����
		--m_iShot;
		// �Ѿ� ����
		Shot();
	}

	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();
	// ������ �� ���� ó��
	Scene_Change();
	// �ִϸ��̼� ������ �̵�
	Frame_Move();

	return OBJ_NOEVENT;
}

void CBlue::Late_Update()
{
	// ��� ���� ������Ʈ�� �ʱ�ȭ �� ���� ����
	if (m_bHpLock)
	{
		// ���ʹ� �ϴ� ȭ�鿡 ���̰� ���� ���� ���� ��
		if (0 <= m_tRect.bottom && 0 <= m_tRect.right && WINCX >= m_tRect.left)
		{
			// ���� ���� ����
			m_bHpLock = false;
			// �Ѿ� �ѹ� ����
			m_iShot = 1;
		}
	}
}

void CBlue::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Blue");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// �浹 �ڽ�
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CBlue::Release()
{
}

// ������ �� ���� ó��
void CBlue::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBlue::FRONT:
			m_tFrame.iFrameScene = CBlue::FRONT;
			break;
		case CBlue::LEFT_1:
			m_tFrame.iFrameScene = CBlue::LEFT_1;
			break;
		case CBlue::LEFT_2:
			m_tFrame.iFrameScene = CBlue::LEFT_2;
			break;
		case CBlue::LEFT_3:
			m_tFrame.iFrameScene = CBlue::LEFT_3;
			break;
		case CBlue::RIGHT_1:
			m_tFrame.iFrameScene = CBlue::RIGHT_1;
			break;
		case CBlue::RIGHT_2:
			m_tFrame.iFrameScene = CBlue::RIGHT_2;
			break;
		case CBlue::RIGHT_3:
			m_tFrame.iFrameScene = CBlue::RIGHT_3;
			break;
		case CBlue::END:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// �Ķ���� ��ǥ�� �̵�
// _fDestX : ������ ��ǥ X
// _fDestY : ������ ��ǥ Y
// _fAngle : ��ȸ ����
// ��ȯ �� : �������� ���������� true �ƴϸ� false
bool CBlue::Move(float _fDestX, float _fDestY, float _fAngle)
{
	float fX = _fDestX - m_tInfo.fX;		// ���� ��ǥ X
	float fY = _fDestY - m_tInfo.fY;		// ���� ��ǥ Y
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

	// ������ �ʹ� ũ��
	// ���� �Ÿ��� �����ϱ� ������ �ʹ� ���ۺ��� ��
	if (abs(_fAngle) > 30 && fDia < 30) {
		// ��������
		_fAngle = _fAngle / 2.f;
	}

	// ��ȸ ���� ���� ��
	fRad = PI / 180.f * _fAngle;
	// ȸ�� �̵��� ��ǥ ��
	fvx2 = cos(fRad) * fvx - sin(fRad) * fvy;
	fvy2 = sin(fRad) * fvx + cos(fRad) * fvy;
	// ������ ���� ���� ���� ��ǥ�� ���ؼ� ��ǥ �̵�
	m_tInfo.fX += fvx2;
	m_tInfo.fY += fvy2;

	// X ��ǥ �̵� ���� ���� ���¸� �ٲ�
	// ���´� ������ �Ҷ� ��������Ʈ �̹����� ���� ������
	if (fvx2 > 4.f) {
		m_eCurState = CBlue::RIGHT_3;
	}
	else if (fvx2 > 2.f) {
		m_eCurState = CBlue::RIGHT_2;
	}
	else if (fvx2 > 0.5f) {
		m_eCurState = CBlue::RIGHT_1;
	}
	else if (fvx2 < -4.f) {
		m_eCurState = CBlue::LEFT_3;
	}
	else if (fvx2 < -2.f) {
		m_eCurState = CBlue::LEFT_2;
	}
	else if (fvx2 < -0.5f) {
		m_eCurState = CBlue::LEFT_1;
	}
	else {
		m_eCurState = CBlue::FRONT;
	}

	// �������� �����ߴ���?
	if (fDia - fSpeed == 0)
		return true;

	return false;
}

// �Ѿ� ����
void CBlue::Shot()
{
	float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;	// ���� ��ǥ X
	float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;	// ���� ��ǥ Y
	float	fDia = sqrtf(fX * fX + fY * fY);			// �Ÿ�
	// arccos(cos) = ���� ���� ����
	float	fRad = acosf(fX / fDia);
	// ���� ������ ȣ������ ����
	float fAngle = fRad * 180.f / PI;
	// Ÿ���� Y ��ǥ�� ���� Y ��ǥ ���� ũ�� ��ȣ ����
	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		fAngle *= -1.f;

	// �ʹ� �Ϻ��ϰ� �÷��̾� ��ġ�� �������� ���ϰ� �ڸ��� ����
	fAngle = (float)((int)fAngle / 10 * 10);
	// �Ѿ� ������Ʈ ����
	CObj* pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX, m_tInfo.fY - 30.f);
	pObj->Set_Angle(fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}
