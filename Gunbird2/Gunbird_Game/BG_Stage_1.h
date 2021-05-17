#pragma once

#ifndef __BG_STAGE_1_H__
#define __BG_STAGE_1_H__

#include "Obj.h"
class CBG_Stage_1 :
	public CObj
{
public:
	CBG_Stage_1();
	virtual ~CBG_Stage_1();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	bool		m_bBossRoom;
};

#endif // !__BG_STAGE_1_H__


