#pragma once

#ifndef __UI_POWERUPDOWNTEXT_H__
#define __UI_POWERUPDOWNTEXT_H__

#include "Obj.h"
class CUI_PowerUpDownText :
	public CObj
{
public:
	CUI_PowerUpDownText();
	virtual ~CUI_PowerUpDownText();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	DWORD			m_dwTime;
	DWORD			m_dwDelay;
};

#endif // !__UI_POWERUPDOWNTEXT_H__


