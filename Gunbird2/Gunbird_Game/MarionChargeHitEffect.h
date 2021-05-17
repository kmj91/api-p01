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


