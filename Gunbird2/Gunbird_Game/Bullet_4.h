// �����
// �� �Ѿ� 4
// ������ ���� ���� �����Ǽ� �߻��

#pragma once

#ifndef __BULLET_4_H__
#define __BULLET_4_H__

#include "Bullet.h"
class CBullet_4 :
	public CBullet
{
public:
	CBullet_4();
	virtual ~CBullet_4();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__BULLET_4_H__


