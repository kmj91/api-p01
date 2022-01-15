// 기명준
// 충전 이펙트

#pragma once

#ifndef __EFFECT_CHARGE_H__
#define __EFFECT_CHARGE_H__

#include "Effect.h"
class CEffect_Charge :
	public CEffect
{
public:
	CEffect_Charge();
	virtual ~CEffect_Charge();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__EFFECT_CHARGE_H__


