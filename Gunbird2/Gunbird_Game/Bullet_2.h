#pragma once

#ifndef __BULLET_2_H__
#define __BULLET_2_H__

#include "Bullet.h"
class CBullet_2 :
	public CBullet
{
public:
	CBullet_2();
	virtual ~CBullet_2();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__BULLET_2_H__


