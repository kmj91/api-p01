#pragma once

#ifndef __SIDECONTINUE_H__
#define __SIDECONTINUE_H__

#include "Obj.h"

class CSideContinue final :
	public CObj
{
public:
	CSideContinue();
	virtual ~CSideContinue();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	DWORD	m_dwCountdown;	// 숫자 변경 시간
};

#endif // !__SIDECONTINUE_H__


