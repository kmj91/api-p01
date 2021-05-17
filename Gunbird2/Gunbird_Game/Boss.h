#pragma once

#ifndef __BOSS_H__
#define __BOSS_H__

#include "Monster.h"

class CBoss abstract :
	public CMonster
{
public:
	explicit CBoss();
	virtual ~CBoss();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	virtual void Check_Damage(int _iPlayerNum) = 0;	// 보스 부위별 파괴 데미지 체크

public:
	void Update_Rect();		// 보스는 여러개의 렉트를 가지고 있음 Obj 거를 호출 하지 않고 이걸 한다				
	bool Check_Hit(const RECT* _stpBullet, float _fDamage, int _iPlayerNum);	// 일반 공격에의한 피해 - 보스의 렉트를 검사해서 부분 피해를 받는다
	void All_Hit(float _fDamage, int _iPlayerNum);	// 폭탄같은 전체 공격에 의한 피해

protected:
	vector<RECT>		m_vecHitRectPos;	// 오브젝트 이미지 기준 히트박스 위치
	vector<RECT>		m_vecHitRect;		// 오브젝트 위치 값을 적용한 히트박스
	vector<float>		m_vecRectHp;		// 각각의 렉트 부분 체력
	bool				m_bDestroy;			// 파괴 플래그
	bool				m_bAttack;			// 공격 플래그
};

#endif // !__BOSS_H__


