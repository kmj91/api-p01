// 기명준
// 몬스터 파괴 이펙트
// 딜레이 값에 따라 지연되서 터지거나 함

#pragma once

#ifndef __CEXPLOSION_02_H__
#define __CEXPLOSION_02_H__

#include "Effect.h"
class CExplosion_02_1 :
	public CEffect
{
public:
	CExplosion_02_1();
	virtual ~CExplosion_02_1();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__CEXPLOSION_02_H__


