// 기명준
// 폭탄 횟수 증가 아이템
// 화면 안에서 특정 각도로 부딪히며 돌아다님

#pragma once

#ifndef __BOMB_H__
#define __BOMB_H__

#include "Item.h"
class CBomb :
	public CItem
{
public:
	CBomb();
	virtual ~CBomb();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	float m_fRadian;	// 이동 각도 라디안
};

#endif // !__BOMB_H__


