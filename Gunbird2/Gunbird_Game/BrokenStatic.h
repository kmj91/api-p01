#pragma once

#ifndef __BROKENSTATIC_H__
#define __BROKENSTATIC_H__

#include "Obj.h"
class CBrokenStatic :
	public CObj
{
public:
	CBrokenStatic();
	virtual ~CBrokenStatic();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Update_RectEx();
};

#endif // !__BROKENSTATIC_H__


