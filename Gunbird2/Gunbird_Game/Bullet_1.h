// 기명준
// 적 총알 1
// 딜레이 값에 따라 지연되서 발사됨

#pragma once

#ifndef __BULLET_1_H__
#define __BULLET_1_H__

#include "Bullet.h"
class CBullet_1 :
	public CBullet
{
public:
	CBullet_1();
	virtual ~CBullet_1();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__BULLET_1_H__


