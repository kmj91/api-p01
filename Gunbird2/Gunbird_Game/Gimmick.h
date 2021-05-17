#pragma once

#ifndef __GIMMICK_H__
#define __GIMMICK_H__

#include "NPC.h"
class CGimmick :
	public CNPC
{
private:
	enum STATE { MOVE_U, MOVE_RU, MOVE_R, MOVE_RD, MOVE_D, MOVE_LD, MOVE_L, MOVE_LU, IDLE_L, IDLE, IDLE_R, IDLE_2, END };

public:
	CGimmick();
	virtual ~CGimmick();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Scene_Change();
	void Move();

private:
	CGimmick::STATE			m_ePreState;
	CGimmick::STATE			m_eCurState;
};

#endif // !__GIMMICK_H__


