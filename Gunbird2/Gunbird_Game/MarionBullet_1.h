#pragma once

#ifndef __MARIONBULLETPOWER_1_H__
#define __MARIONBULLETPOWER_1_H__

#include "Bullet.h"
class CMarionBullet_1 :
	public CBullet
{
public:
	CMarionBullet_1();
	virtual ~CMarionBullet_1();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__MARIONBULLETPOWER_1_H__


