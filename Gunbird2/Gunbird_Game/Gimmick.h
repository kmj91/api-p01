// 기명준
// NPC 기믹 배경 취급
// 게임 플레이 중간에 모습을 보임

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
	void Scene_Change();	// 프레임 씬 변경 처리
	void Move();			// 이동

private:
	CGimmick::STATE			m_ePreState;
	CGimmick::STATE			m_eCurState;
};

#endif // !__GIMMICK_H__


