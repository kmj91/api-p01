#pragma once

#ifndef __SHARK_H__
#define __SHARK_H__

#include "NPC.h"
class CShark :
	public CNPC
{
private:
	enum STATE { MOVE_U, MOVE_RU, MOVE_R, MOVE_RD, MOVE_D, MOVE_LD, MOVE_L, MOVE_LU, IDLE_L, IDLE, IDLE_R, IDLE_2, END };

public:
	CShark();
	virtual ~CShark();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Scene_Change();
	void Move();

private:
	CShark::STATE			m_ePreState;
	CShark::STATE			m_eCurState;
};

#endif // !__SHARK_H__


