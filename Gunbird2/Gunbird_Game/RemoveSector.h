#pragma once

#ifndef __REMOVESECTOR_H__
#define __REMOVESECTOR_H__

#include "Obj.h"
class CRemoveSector :
	public CObj
{
public:
	CRemoveSector();
	virtual ~CRemoveSector();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__REMOVESECTOR_H__


