// 기명준
// 마리온 근접 공격 명중 이펙트

#pragma once

#ifndef __EFFECT_MARIONMELEEHIT_H__
#define __EFFECT_MARIONMELEEHIT_H__

#include "Effect.h"
class CEffect_MarionMeleeHit :
	public CEffect
{
public:
	CEffect_MarionMeleeHit();
	virtual ~CEffect_MarionMeleeHit();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__EFFECT_MARIONMELEEHIT_H__


