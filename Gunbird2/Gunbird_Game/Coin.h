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

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__COIN_H__


