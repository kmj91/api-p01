#pragma once

#ifndef __MARIONCHARGEBULLET_H__
#define __MARIONCHARGEBULLET_H__

#include "Bullet.h"
class CMarionChargeBullet :
	public CBullet
{
private:
	enum STATE { START, REPEAT, END };

public:
	CMarionChargeBullet();
	virtual ~CMarionChargeBullet();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Scene_Change();

private:
	CMarionChargeBullet::STATE		m_ePreState;
	CMarionChargeBullet::STATE		m_eCurState;
};

#endif // !__MARIONCHARGEBULLET_H__


