// 기명준
// 게임 시작시 처음 화면에 보이는 타이틀 로고

#pragma once

#ifndef __UI_TITLELOGO_H__
#define __UI_TITLELOGO_H__

#include "Obj.h"
class CUI_TitleLogo :
	public CObj
{
private:
	enum STATE { ON_TITLE_LOGO, BLEND_LOGO, MOVE_LOGO, END };

public:
	CUI_TitleLogo();
	virtual ~CUI_TitleLogo();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	CUI_TitleLogo::STATE	m_eState;
	DWORD					m_dwTime;
	DWORD					m_dwDelay;
	BYTE					m_byBlend;			// 투명도 0 ~ 255
	bool					m_bRander;
};

#endif // !__UI_TITLELOGO_H__


