// 기명준
// 플레이어 캐릭터 부모 클래스
// 공통 처리 함수나 멤버 변수를 가짐

#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Obj.h"
class CPlayer abstract :
	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	// 총알 파워 업
	virtual void PowerUp() {}
	// 총알 파워 다운
	virtual void PowerDown(){}
	// 키 입력
	// _Key : 입력된 키 enum 값
	virtual void Key_Input(int _Key) {}

public:
	// 플레이어 번호 반환
	int Get_PlayerNum() { return m_iPlayerNum; }
	// 플레이어 1p, 2p 지정
	// _iNum : 파라미터 값이 1일 경우 1플레이어, 2면 2플레이어
	void Set_PlayerNum(int _iNum) { m_iPlayerNum = _iNum; }

protected:
	PLAYER::STATE		m_ePreState;
	PLAYER::STATE		m_eCurState;
	RECT				m_tMeleeRect;			// 근접 공격 렉트
	DWORD				m_dwKeyTime;			// 키 입력 시간
	DWORD				m_dwKeyDelay;			// 키 입력 딜레이
	DWORD				m_dwBulletTime;			// 총알 발싸 시간
	DWORD				m_dwBulletDelay;		// 총알 발싸 딜레이
	DWORD				m_dwRespawnTime;		// 리스폰 시간
	DWORD				m_dwRespawnDelay;		// 리스폰 딜레이
	DWORD				m_dwHpLockTime;			// 무적 시간
	DWORD				m_dwHpLockDelay;		// 무적 딜레이
	DWORD				m_dwChargeTime;			// 충전 공격 키 입력 시간
	DWORD				m_dwChargeDelay;		// 충전 공격 키 입력 딜레이
	DWORD				m_dwChargeBulletTime;	// 충전 공격 시간
	DWORD				m_dwChargeBulletDelay;	// 충전 공격 딜레이
	DWORD				m_dwPowerDownTime;		// 파워 다운 시간
	DWORD				m_dwPowerDownDelay;		// 파워 다운 딜레이
	int					m_iPlayerNum;			// 플레이어 번호	(1P or 2P)
	int					m_iShotPower;			// 총알 파워
	int					m_iShotCnt;				// 총알 개수
	int					m_iChargeBulletCnt;		// 충전샷 개수
	float				m_fAngelY;				// 플레이어 좌우 이동 할 때의 기울기 축
	float				m_fMaxRotAngle;			// 최대 선회 각도
	float				m_fRotSpeed;			// 회전 속도
	bool				m_bRespawn;				// 리스폰 중
	bool				m_bOnShot;				// 공격 버튼 누르고 있는가?
	bool				m_bImmutable;			// 스프라이트 고정 - 다른 키 조작이나 동작으로 스프라이트가 교체되는걸 막는 플래그
};

#endif // !__PLAYER_H__


