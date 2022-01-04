// 기명준
// 모든 게임 오브젝트들의 부모

#include "stdafx.h"
#include "Obj.h"

CObj::CObj()
	: m_tInfo{}, m_tRect{}, m_tHitRectPos{}, m_tHitRect{}, m_tFrame{}, m_fSpeed(0.f), m_fAngle(0.f), m_pTarget(nullptr), m_pFrameKey(nullptr),
	m_iDrawID(0), m_iImageWidth(0), m_iImageHeight(0), m_fHp(1.f), m_bDead(false), m_bHpLock(true), m_bRemove(false)
{
}

CObj::~CObj()
{
}

// 공격 받음
// 파라미터 값 만큼 체력 차감, 체력이 0 이하가되면 m_bDead가 true로
// _fDamage : 받은 피해량
void CObj::Hit(float _fDamage)
{
	m_fHp -= _fDamage;
	if (m_fHp <= 0.f)
		m_bDead = true;
}

// 현재 오브젝트 좌표를 기준으로 이미지 렉트 정보 갱신
void CObj::Update_Rect()
{
	// 이미지의 길이를 반으로 나눈 값을 저장합니다.
	LONG lCX = LONG(m_tInfo.iCX >> 1);
	LONG lCY = LONG(m_tInfo.iCY >> 1);
	
	// 게임 오브젝트들의 좌표는 이미지의 중심입니다.
	// 게임 오브젝트의 좌표에서 위에서 구한 값을 빼고 더해서 이미지의 Rect를 구합니다.
	m_tRect.left = (LONG)m_tInfo.fX - lCX;
	m_tRect.top = (LONG)m_tInfo.fY - lCY;
	m_tRect.right = (LONG)m_tInfo.fX + lCX;
	m_tRect.bottom = (LONG)m_tInfo.fY + lCY;

	// 오브젝트의 왼쪽 상단 꼭지점 좌표를 구합니다.
	LONG lX = (LONG)m_tInfo.fX - lCX;
	LONG lY = (LONG)m_tInfo.fY - lCY;

	// 위에서 구한 왼쪽 상단 좌표와
	// 이미지 기준(왼쪽 상단이 0,0인) 로컬 충돌 범위 Rect를 더해서
	// 현재 좌표를 기준으로 하는 월드 충돌 범위를 구합니다.
	m_tHitRect.left = lX + m_tHitRectPos.left;
	m_tHitRect.top = lY + m_tHitRectPos.top;
	m_tHitRect.right = lX + m_tHitRectPos.right;
	m_tHitRect.bottom = lY + m_tHitRectPos.bottom;
}

// 이미지 프레임 이동
void CObj::Frame_Move()
{
	// 시작 프레임이 끝 프레임 보다 작다면 (ex. 시작 0 -> 끝 10)
	// 정상적인 순서로 프레임을 이동합니다.
	if (m_tFrame.iFrameStart <= m_tFrame.iFrameEnd)
	{
		// 이전 프레임 초기화 시간 + 프레임 재생 속도(딜레이) 값이 지금 시간 보다 작다면
		// 충분한 시간이 지났으니 다음 프레임으로 이동합니다.
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			// 프레임 인덱스 증가
			++m_tFrame.iFrameCnt;
			// 만약 증가한 프레임 값이 프레임 끝보다 크다면 다시 시작 프레임으로 초기화 해줍니다.
			if (m_tFrame.iFrameCnt > m_tFrame.iFrameEnd)
				m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			// 프레임 이동 후 현재 시간으로 초기화 해줍니다.
			m_tFrame.dwFrameTime = GetTickCount();
		}
	}
	// 만약 반대면 (ex. 시작 10 -> 끝 0)
	// 역순으로 프레임을 감소하면서 이동합니다.
	else
	{
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			// 프레임 인덱스 감소
			--m_tFrame.iFrameCnt;
			// 만약 감소한 프레임이 끝 프레임보다 작다면 시작 프레임으로 초기화
			if (m_tFrame.iFrameCnt < m_tFrame.iFrameEnd)
				m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.dwFrameTime = GetTickCount();
		}
	}
}
