#pragma once

#ifndef __MARIONGUIDEDBULLET_H__
#define __MARIONGUIDEDBULLET_H__

#include "Bullet.h"
class CMarionGuidedBullet :
	public CBullet
{
private:
	enum ANGLE {
		A90, A70, A45, A20,
		A0, A_20, A_45, A_70,
		A_90, A_110, A_135, A_160,
		A180, A160, A135, A110,
		END
	};

public:
	CMarionGuidedBullet();
	virtual ~CMarionGuidedBullet();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Init_Angle();

	void Set_BulletCnt(BULLET * _stpSubBulletCnt) { m_stpSubBulletCnt = _stpSubBulletCnt; }

private:
	DWORD m_dwEffectCreateTime;		// 이펙트 생성 시간
	DWORD m_dwEffectCreateDelay;	// 이펙트 생성 딜레이
	BULLET * m_stpSubBulletCnt;
};

#endif // !__MARIONGUIDEDBULLET_H__


