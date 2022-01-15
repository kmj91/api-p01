// 기명준
// 마리온 별 이펙트

#pragma once

#ifndef __MARIONEFFECT_H__
#define __MARIONEFFECT_H__

#include "Effect.h"
class CMarionEffect :
	public CEffect
{
public:
	CMarionEffect();
	virtual ~CMarionEffect();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__MARIONEFFECT_H__


