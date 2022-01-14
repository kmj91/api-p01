// 기명준
// 플레이어 총알이 명중하면 생성되는 이펙트
// 좌우 반전된 두 종류의 이미지를 사용 함

#pragma once

#ifndef __BULLETHIT_H__
#define __BULLETHIT_H__

#include "Effect.h"
class CBulletHit :
	public CEffect
{
public:
	CBulletHit();
	virtual ~CBulletHit();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__BULLETHIT_H__


