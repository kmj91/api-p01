// 기명준
// 적 몬스터 로켓
// 별다른 공격 능력은 없고 아래쪽을 향해 이동

#pragma once

#ifndef __ROCKET_H__
#define __ROCKET_H__

#include "Monster.h"
class CRocket :
	public CMonster
{
public:
	CRocket();
	virtual ~CRocket();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__ROCKET_H__


