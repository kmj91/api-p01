#pragma once

#ifndef __MARIONBULLET_3_H__
#define __MARIONBULLET_3_H__

#include "Bullet.h"
class CMarionBullet_3 :
	public CBullet
{
public:
	CMarionBullet_3();
	virtual ~CMarionBullet_3();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__MARIONBULLET_3_H__


