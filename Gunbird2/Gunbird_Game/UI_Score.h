#pragma once

#ifndef __UI_SCORE_H__
#define __UI_SCORE_H__

#include "Obj.h"
class CUI_Score final :
	public CObj
{
public:
	CUI_Score();
	virtual ~CUI_Score();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	int Get_Score() { return m_iScore; }

	void Add_Score(int _iScore) { m_iScore += _iScore; }

private:
	int		m_iScore;
};

#endif // !__UI_SCORE_H__


