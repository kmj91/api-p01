// 기명준
// 보스 배기구 연기
// 왼쪽 오른쪽 2개 방향으로 있음
// 보스 위치에 상대적으로 좌표 값을 가짐

#pragma once

#ifndef __PIPESMOKE_H__
#define __PIPESMOKE_H__

#include "Effect.h"
class CPipeSmoke :
	public CEffect
{
public:
	CPipeSmoke();
	virtual ~CPipeSmoke();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__PIPESMOKE_H__


