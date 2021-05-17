#pragma once

#ifndef __UI_SCORETEXT_H__
#define __UI_SCORETEXT_H__

#include "Obj.h"
class CUI_ScoreText :
	public CObj
{
public:
	CUI_ScoreText();
	virtual ~CUI_ScoreText();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	DWORD			m_dwTime;
	DWORD			m_dwDelay;
};

#endif // !__UI_SCORETEXT_H__


