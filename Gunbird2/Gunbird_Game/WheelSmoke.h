// 기명준
// 보스 바퀴 연기

#pragma once

#ifndef __WHEELSMOKE_H__
#define __WHEELSMOKE_H__

#include "Effect.h"
class CWheelSmoke :
	public CEffect
{
public:
	CWheelSmoke();
	virtual ~CWheelSmoke();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__WHEELSMOKE_H__


