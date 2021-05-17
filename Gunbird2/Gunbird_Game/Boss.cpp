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

// 보스는 여러개의 렉트를 가지고 있음 Obj 거를 호출 하지 않고 이걸 한다
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

// 일반 공격에의한 피해 - 보스의 렉트를 검사해서 부분 피해를 받는다
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
			Hit(_fDamage);	// 본체 체력
			m_vecRectHp[iCnt] -= _fDamage;	// 부분 체력
			Check_Damage(_iPlayerNum);		// 부위 파괴 확인
			return true;
		}
		++iCnt;
	}

	return false;
}

// 폭탄같은 전체 공격에 의한 피해
void CBoss::All_Hit(float _fDamage, int _iPlayerNum)
{
	if (m_bHpLock) {
		return;
	}

	int iSize = m_vecHitRect.size();
	int iCnt = 0;
	while (iCnt < iSize) {
		// 원작에서 폭탄만 써서 공격할 경우
		// 부분 파괴는 잘되는데 보스 본체 체력은 잘 안깍이는 느낌임
		// 그래서 이런식으로 데미지를 깍음
		Hit(_fDamage);	// 본체 체력
		m_vecRectHp[iCnt] -= _fDamage;	// 부분 체력
		Check_Damage(_iPlayerNum);		// 부위 파괴 확인
		++iCnt;
	}
}
