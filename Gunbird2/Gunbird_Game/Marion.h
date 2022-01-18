// 기명준
// 플레이어 마리온
// 몇몇 애니메이션 프레임의 이미지 길이가 달라서
// 상태가 변경된 후 위치 좌표를 초기화할 때 주의해야 함

#pragma once

#ifndef __MARION_H__
#define __MARION_H__

#include "Player.h"
class CMarion final :
	public CPlayer
{
public:
	CMarion();
	virtual ~CMarion();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update();
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	// 총알 파워 업
	virtual void PowerUp() override;
	// 총알 파워 다운
	virtual void PowerDown() override;
	// 키 입력
	// _Key : 입력된 키 enum 값
	virtual void Key_Input(int _Key);
	// 캐릭터 이동
	virtual void Move_LL();
	virtual void Move_LU();
	virtual void Move_LD();
	virtual void Move_RR();
	virtual void Move_RU();
	virtual void Move_RD();
	virtual void Move_UU();
	virtual void Move_DD();
	// 이동 끝
	virtual void Move_End();
	// 일반 공격
	virtual void Attack();
	// 일반 공격 끝
	virtual void Attack_End();
	// 폭탄 공격
	virtual void Bomb();
	// 근접 공격
	virtual void Melee();
	// 다시 하기
	virtual void Continue();

private:
	void Scene_Change();			// 프레임 씬 변경 처리
	void Move_Change();				// 이동 상태 변경 처리
	void Shot();					// 일반 공격 처리
	void Create_Effect();			// 별 이펙트 생성
	bool CanKeyInput();				// 키 처리가 가능한지 검사
	bool CanAttack();				// 공격할 수 있는지 검사

	template <typename T>
	CObj* Create_Bullet(float _fX, float _fY, float _fAngle)
	{
		return CAbstractFactory<T>::Create(_fX, _fY, _fAngle);
	}

private:
	DWORD		m_dwEffectCreateTime;	// 이펙트 생성 시간
	DWORD		m_dwEffectCreateDelay;	// 이펙트 생성 딜레이
	bool		m_bBombEffect;			// 폭탄 이펙트용 변수 - 번갈아가면서 위치 변경
	bool		m_bMeleeHit;			// 근접 공격 적중 확인용
	BULLET		m_tSubBulletCnt;		// 서브 유도 미사일용 구조체
};

#endif // !__MARION_H__

