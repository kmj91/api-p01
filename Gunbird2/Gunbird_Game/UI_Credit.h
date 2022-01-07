// 기명준
// UI 크레딧

#pragma once

#ifndef __UI_CREDIT_H__
#define __UI_CREDIT_H__

#include "Obj.h"
class CUI_Credit final :
	public CObj
{
public:
	CUI_Credit();
	virtual ~CUI_Credit();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	int Get_Credit() { return m_iCredit; }
	void Set_Credit(int _iCredit) { m_iCredit = _iCredit; }

	void Add_Credit(int _iCredit) { m_iCredit += _iCredit; }

private:
	int		m_iCredit;
};

#endif // !__UI_CREDIT_H__


