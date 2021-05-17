#pragma once

#ifndef __PLAYERDEAD_H__
#define __PLAYERDEAD_H__

#include "Effect.h"
class CPlayerDead :
	public CEffect
{
public:
	CPlayerDead();
	virtual ~CPlayerDead();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__PLAYERDEAD_H__


