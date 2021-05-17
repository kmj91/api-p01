#pragma once

#ifndef __PIPESMOKE_H__
#define __PIPESMOKE_H__

#include "Effect.h"
class CPipeSmoke :
	public CEffect
{
public:
	CPipeSmoke();
	virtual ~CPipeSmoke();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__PIPESMOKE_H__


