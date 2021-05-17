#pragma once

#ifndef __BOMB_H__
#define __BOMB_H__

#include "Item.h"
class CBomb :
	public CItem
{
public:
	CBomb();
	virtual ~CBomb();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	float m_fRadian;
};

#endif // !__BOMB_H__


