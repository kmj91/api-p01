// 기명준
// 몬스터 부모 클래스
// 플레이어가 점수를 얻을 때 필요한 멤버 변수를 공통으로 가지고있음

#pragma once

#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Obj.h"
class CMonster :
	public CObj
{
public:
	CMonster();
	virtual ~CMonster();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	int Get_Score() { return m_iScore; }

protected:
	int m_iScore;		// 파괴시 플레이어가 얻을 점수 값
};

#endif // !__ROCKET_H__


