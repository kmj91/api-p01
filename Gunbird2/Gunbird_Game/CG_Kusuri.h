#pragma once

#ifndef __CG_KUSURI_H__
#define __CG_KUSURI_H__

#include "Obj.h"
class CCG_Kusuri :
	public CObj
{
public:
	CCG_Kusuri();
	virtual ~CCG_Kusuri();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	int		m_iBGFrameCnt;
	bool	m_bFlag;
};

#endif // !__CG_KUSURI_H__


