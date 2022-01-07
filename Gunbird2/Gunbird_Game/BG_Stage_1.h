// 기명준
// 게임 스테이지 배경 스크롤
// 보스전이 Y축 좌표 몇 부터 시작하는지에 대해 처리
// 보스전부터 일정 길이의 배경 스크롤이 반복 처리

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


