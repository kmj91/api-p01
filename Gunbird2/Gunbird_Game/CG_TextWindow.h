#pragma once

#ifndef __CG_TEXTWINDOW_H__
#define __CG_TEXTWINDOW_H__

#include "Obj.h"
class CCG_TextWindow :
	public CObj
{
private:
	enum STATE { ON_TEXTBOX, TEXT_1, TEXT_2, GAME_END, END };

public:
	CCG_TextWindow();
	virtual ~CCG_TextWindow();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Update_RectEX();

private:
	CCG_TextWindow::STATE			m_eState;
	DWORD							m_dwTime;
	DWORD							m_dwDelay;
	INFO							m_tStandingInfo[2];
	RECT							m_tStandingRect[2];
	BYTE							m_byBlend;			// ≈ı∏Ìµµ 0 ~ 255
};

#endif // !__CG_TEXTWINDOW_H__


