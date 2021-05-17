/**************************************************

메인 컨티뉴
1Player 2Player 모두 사망하였을 때

***************************************************/

#pragma once

#ifndef __UI_MAINCONTINUE_H__
#define __UI_MAINCONTINUE_H__

#include "Obj.h"

class CUI_MainContinue final :
	public CObj
{
public:
	CUI_MainContinue();
	virtual ~CUI_MainContinue();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	// 메인 컨티뉴 카운트 시작
	void Start_Countdown();
	// 메인 컨티뉴 카운트 정지
	void Stop_Countdown();
	// 카운트 감소
	void NextCount();

private:
	// 컨티뉴 시간 감소
	void Update_Count();

private:
	// UI를 화면에 출력할지 결정하는 플래그
	bool m_bOnUI;
};

#endif // !__UI_MAINCONTINUE_H__


