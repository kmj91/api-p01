// 기명준
// 마리온 총알 파워 레벨 1

#pragma once

#ifndef __MARIONBULLET_2_H__
#define __MARIONBULLET_2_H__

#include "Bullet.h"
class CMarionBullet_2 :
	public CBullet
{
public:
	CMarionBullet_2();
	virtual ~CMarionBullet_2();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__MARIONBULLET_2_H__


