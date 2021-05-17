#pragma once

#ifndef __MARIONBULLET_4_H__
#define __MARIONBULLET_4_H__

#include "Bullet.h"
class CMarionBullet_4 :
	public CBullet
{
public:
	CMarionBullet_4();
	virtual ~CMarionBullet_4();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__MARIONBULLET_4_H__


