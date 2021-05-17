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

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	DWORD	m_dwCountdown;	// ���� ���� �ð�
};

#endif // !__SIDECONTINUE_H__


