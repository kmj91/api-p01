#pragma once

#ifndef __TEQUILAGALLOP_H__
#define __TEQUILAGALLOP_H__

#include "Boss.h"
class CTequilaGallop :
	public CBoss
{
private:
	enum class STATE { WAIT, PHASE_MOVE, PHASE_CHANGE,
		READY,
		NEXT_PATTERN,		// 다음 패턴 확인
		L_ARM_ATTACK,		// 왼팔 공격
		R_ARM_ATTACK,		// 오른팔 공격
		FRONT_ATTACK,		// 가슴 공격
		BACK_ATTACK,		// 등 공격
		SHOULDER_ATTACK,	// 어깨 공격
		DESTROY,
		FINAL_1,
		FINAL_2,
		FINAL_3,
		FINAL_4,
		FINAL_5,
		END };

	enum class SPRITE { BODY, L_ARM, R_ARM, END};

public:
	CTequilaGallop();
	virtual ~CTequilaGallop();

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
	void Frame_MoveEX();	// 부분 스프라이트 프레임 업데이트
	void Action();			// 보스 행동
	void Move();			// 이동
	void DamageEffect();	// 파괴 이펙트
	void Shake_Body();		// 상체 흔들기
	void Pipe_Smoke();		// 배기구 이펙트 출력
	void L_Arm_Shot();		// 왼팔 공격
	void R_Arm_Shot();		// 오른팔 공격
	void Front_Shot(float _fX, float _fY);		// 가슴 공격
	void Back_Shot(float _fX, float _fY);		// 등 공격
	void Shoulder_Shot();	// 어깨 공격
	void Shot_5(float _fAngle);

private:
	STATE			m_ePreState;		// 현재 상태
	STATE			m_eCurState;		// 교체할 상태
	DWORD			m_dwShotCnt;		// 총알 쏘는 횟수
	DWORD			m_dwShotTime;		// 총알 쏘는 시간
	DWORD			m_dwShotDelay;		// 총알 쏘는 딜레이
	float			m_fShotAngle;		// 총알 쏘는 각도
	float			m_fBodyPosY;		// 상체 Y 좌표 - m_tInfo 좌표 + 해당 좌표
	float			m_fArmPosY;			// 팔 Y 좌표 - m_tInfo 좌표 + 해당 좌표
	DWORD			m_dwShakeTime;		// 흔들기 시간
	DWORD			m_dwShakeDelay;		// 흔들기 딜레이
	DWORD			m_dwMoveTime;		// 이동 시간
	DWORD			m_dwMoveDelay;		// 이동 딜레이
	DWORD			m_dwUnderFrameCnt;	// 하체 프레임 카운트 - 몇번을 주기로 덜컹 덜컹 거려야됨
	DWORD			m_dwWhellSmokeTime;	// 바퀴 연기 시간
	DWORD			m_dwWhellSmokeDelay;// 바퀴 연기 딜레이
	DWORD			m_dwChargeTime;		// 충전 공격 시간
	DWORD			m_dwChargeDelay;	// 충전 공격 딜레이
	DWORD			m_dwSoundTime;		// 소리 시간
	DWORD			m_dwSoundDelay;		// 소리 딜레이
	DWORD			m_dwPatternIndex;	// 패턴 인덱스
	vector<STATE>	m_vecPattern;		// 패턴을 보관하는 백터
	FRAME			m_tSubFrameArr[static_cast<UINT>(SPRITE::END)];		// 부분 스프라이트 팔 프레임
	DAMAGE			m_bDamageFlagArr[static_cast<UINT>(SPRITE::END)];	// 부분 프레임의 데미지 플래그
	bool			m_bMoveFlag;		// 이동 플래그 - 좌우 이동하는데 일정시간 대기후 반대방향으로 이동함
	bool			m_bDamageLeftMove;	// 왼쪽 이동
	bool			m_bDamageRightMove;	// 오른쪽 이동
};

#endif // !__TEQUILAGALLOP_H__


