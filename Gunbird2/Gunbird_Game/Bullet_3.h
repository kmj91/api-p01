// 기명준
// 적 총알 2
// 딜레이 값에 따라 지연되서 발사됨

#pragma once

#ifndef __BULLET_3_H__
#define __BULLET_3_H__

#include "Bullet.h"
class CBullet_3 :
	public CBullet
{
private:
	enum ANGLE { A0, A_10, A_20, A_30, A_45, A_60, A_75,
		A_90, A_105, A_120, A_135, A_150, A_160, A_170,
		A180, A170, A160, A150, A135, A120, A105,
		A90, A75, A60, A45, A30, A20, A10,
		END };

public:
	CBullet_3();
	virtual ~CBullet_3();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__BULLET_3_H__


