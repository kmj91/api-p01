// 기명준
// 파워업 아이템
// 화면 안에서 특정 각도로 부딪히며 돌아다님
// 파괴자 호출되면 파워업 텍스트 오브젝트 생성

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
	float m_fRadian;	// 이동 각도 라디안
};

#endif // !__POWERUP_H__


