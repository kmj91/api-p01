// 기명준
// 몬스터 파괴 이펙트
// 딜레이 값에 따라 지연되서 터지거나 함

#pragma once

#ifndef __EXPLOSION_03_H__
#define __EXPLOSION_03_H__

#include "Effect.h"
class CExplosion_03 :
	public CEffect
{
public:
	CExplosion_03();
	virtual ~CExplosion_03();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__EXPLOSION_03_H__


