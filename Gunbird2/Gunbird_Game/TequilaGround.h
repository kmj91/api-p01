#pragma once

#ifndef __TEQUILAGROUND_H__
#define __TEQUILAGROUND_H__

#include "Boss.h"
class CTequilaGround :
	public CBoss
{
private:
	enum class STATE { WAIT, CLOSE_DOOR, PAVED_ROAD,
		IDLE,
		ATTACK,
		DESTROY,
		FINAL_1,
		FINAL_2,
		FINAL_3,
		FINAL_4,
		FINAL_5,
		END };

	enum class SPRITE { L_CANNON, R_CANNON, LL_CANNON, RR_CANNON, CENTER_CANNON, END};
	enum class PATTERN { IDLE, L_SUB, R_SUB, LL_SUB, RR_SUB, FRONT, BACK, END };

public:
	CTequilaGround();
	virtual ~CTequilaGround();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	// CBoss을(를) 통해 상속됨
	virtual void Check_Damage(int _iPlayerNum) override;	// 보스 부위별 파괴 데미지 체크

private:
	void Scene_Change();
	void Frame_MoveEX();		// 부분 스프라이트 캐논 프레임 업데이트
	void Action();				// 보스 행동
	void Move();				// 보스 이동
	void DamageEffect();		// 파괴 이펙트
	void L_SideSub_Shot();		// 왼쪽 서브 캐논
	void R_SideSub_Shot();		// 오른쪽 서브 캐논
	void L_SideSub_Buckshot();	// 왼쪽 서브 캐논 산탄
	void R_SideSub_Buckshot();	// 오른쪽 서브 캐논 산탄
	void Front_Shot();			// 정면 캐논
	void L_FrontSub_Shot();		// 왼쪽 정면 서브 캐논
	void R_FrontSub_Shot();		// 오른쪽 정면 서브 캐논
	void Back_AutoShot();		// 뒤쪽 연발 공격
	void Back_Buckshot();		// 뒤쪽 산탄 공격
	void Shot_4();
	void Shot_5(float _fAngle);

private:
	STATE		m_ePreState;		// 현재 상태
	STATE		m_eCurState;		// 교채할 상태
	float		m_fShotAngle;		// 총알 각도
	DWORD		m_dwShotTime;		// 시간
	DWORD		m_dwShotDelay;		// 딜레이
	DWORD		m_dwDestroyTime;	// 파괴 이펙트 시간
	DWORD		m_dwDestroyDelay;	// 파괴 이펙트 딜레이
	DWORD		m_dwMoveTime;		// 이동 시간
	DWORD		m_dwMoveDelay;		// 이동 딜레이
	DELAY		m_tPatternDelay[static_cast<UINT>(PATTERN::END)];	// 패턴 딜레이
	FRAME		m_tCannonFrameArr[static_cast<UINT>(SPRITE::END)];	// 부분 스프라이트 캐논의 프레임
	DAMAGE		m_bDamageFlagArr[static_cast<UINT>(SPRITE::END)];	// 부분 프레임의 데미지 플래그
	bool		m_bBackAttack;		// 뒤쪽 공격 패턴 번갈아가면서 사용하기 위한 플래그
	bool		m_bMoveFlag;		// 이동 플래그 - 좌우 이동하는데 일정시간 대기후 반대방향으로 이동함
};

#endif // !__TEQUILAGROUND_H__


