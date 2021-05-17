#pragma once

#ifndef __EXPLOSION_05_H__
#define __EXPLOSION_05_H__

#include "Effect.h"
class CExplosion_05 :
	public CEffect
{
public:
	CExplosion_05();
	virtual ~CExplosion_05();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__EXPLOSION_05_H__


