#pragma once

#ifndef __STATICOBJ_H__
#define __STATICOBJ_H__

#include "Obj.h"
class CStaticObj :
	public CObj
{
public:
	CStaticObj();
	virtual ~CStaticObj();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Update_RectEx();
};

#endif // !__STATICOBJ_H__


