#pragma once

#ifndef __COIN_H__
#define __COIN_H__

#include "item.h"
class CCoin :
	public CItem
{
public:
	CCoin();
	virtual ~CCoin();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__COIN_H__


