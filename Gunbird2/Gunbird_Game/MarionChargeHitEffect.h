// 기명준
// 마리온 충전 공격 명중 이펙트

#pragma once

#ifndef __MARIONCHARGEHITEFFECT_H__
#define __MARIONCHARGEHITEFFECT_H__

#include "Effect.h"
class CMarionChargeHitEffect :
	public CEffect
{
public:
	CMarionChargeHitEffect();
	virtual ~CMarionChargeHitEffect();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__MARIONCHARGEHITEFFECT_H__


