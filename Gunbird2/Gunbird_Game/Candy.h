#pragma once

#ifndef __CANDY_H__
#define __CANDY_H__

#include "item.h"
class CCandy :
	public CItem
{
public:
	CCandy();
	virtual ~CCandy();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	float		m_fForce;
};

#endif // !__CANDY_H__


