// �ı��� ȣ��Ǹ� �Ŀ��� �ؽ�Ʈ ������Ʈ ����

#pragma once

#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "Item.h"
class CPowerUp :
	public CItem
{
public:
	CPowerUp();
	virtual ~CPowerUp();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	float m_fRadian;
};

#endif // !__POWERUP_H__


