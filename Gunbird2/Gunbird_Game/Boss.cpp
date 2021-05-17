#include "stdafx.h"
#include "Boss.h"


CBoss::CBoss() : m_bDestroy(false), m_bAttack(false)
{
}


CBoss::~CBoss()
{
}

void CBoss::Initialize()
{
}

int CBoss::Update()
{
	return 0;
}

void CBoss::Late_Update()
{
}

void CBoss::Render(HDC _DC)
{
}

void CBoss::Release()
{
}

// ������ �������� ��Ʈ�� ������ ���� Obj �Ÿ� ȣ�� ���� �ʰ� �̰� �Ѵ�
void CBoss::Update_Rect()
{
	LONG lCX = LONG(m_tInfo.iCX >> 1);
	LONG lCY = LONG(m_tInfo.iCY >> 1);

	m_tRect.left = (LONG)m_tInfo.fX - lCX;
	m_tRect.top = (LONG)m_tInfo.fY - lCY;
	m_tRect.right = (LONG)m_tInfo.fX + lCX;
	m_tRect.bottom = (LONG)m_tInfo.fY + lCY;

	LONG lX = (LONG)m_tInfo.fX - lCX;
	LONG lY = (LONG)m_tInfo.fY - lCY;
	
	int iSize = m_vecHitRectPos.size();
	int iCnt = 0;
	while (iCnt < iSize) {
		m_vecHitRect[iCnt].left = lX + m_vecHitRectPos[iCnt].left;
		m_vecHitRect[iCnt].top = lY + m_vecHitRectPos[iCnt].top;
		m_vecHitRect[iCnt].right = lX + m_vecHitRectPos[iCnt].right;
		m_vecHitRect[iCnt].bottom = lY + m_vecHitRectPos[iCnt].bottom;
		++iCnt;
	}
}

// �Ϲ� ���ݿ����� ���� - ������ ��Ʈ�� �˻��ؼ� �κ� ���ظ� �޴´�
bool CBoss::Check_Hit(const RECT * _stpBullet, float _fDamage, int _iPlayerNum)
{
	if (m_bHpLock) {
		return false;
	}

	int iSize = m_vecHitRect.size();
	int iCnt = 0;
	RECT rc;
	while (iCnt < iSize) {
		if (IntersectRect(&rc, _stpBullet, &m_vecHitRect[iCnt])) {
			Hit(_fDamage);	// ��ü ü��
			m_vecRectHp[iCnt] -= _fDamage;	// �κ� ü��
			Check_Damage(_iPlayerNum);		// ���� �ı� Ȯ��
			return true;
		}
		++iCnt;
	}

	return false;
}

// ��ź���� ��ü ���ݿ� ���� ����
void CBoss::All_Hit(float _fDamage, int _iPlayerNum)
{
	if (m_bHpLock) {
		return;
	}

	int iSize = m_vecHitRect.size();
	int iCnt = 0;
	while (iCnt < iSize) {
		// ���ۿ��� ��ź�� �Ἥ ������ ���
		// �κ� �ı��� �ߵǴµ� ���� ��ü ü���� �� �ȱ��̴� ������
		// �׷��� �̷������� �������� ����
		Hit(_fDamage);	// ��ü ü��
		m_vecRectHp[iCnt] -= _fDamage;	// �κ� ü��
		Check_Damage(_iPlayerNum);		// ���� �ı� Ȯ��
		++iCnt;
	}
}
