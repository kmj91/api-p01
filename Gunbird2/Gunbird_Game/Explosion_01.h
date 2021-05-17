#pragma once

#ifndef __EXPLOSION_01_H__
#define __EXPLOSION_01_H__

#include "Effect.h"
class CExplosion_01 :
	public CEffect
{
public:
	CExplosion_01();
	virtual ~CExplosion_01();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__EXPLOSION_01_H__


