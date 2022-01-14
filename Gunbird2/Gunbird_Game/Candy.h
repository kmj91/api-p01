// 기명준
// 폭탄 사용시 적 총알이 변환된 사탕 오브젝트
// 순간적으로 떠오르다가 맵 하단으로 쭉 떨어짐
// 충돌하면 점수 획득

#pragma once

#ifndef __CANDY_H__
#define __CANDY_H__

#include "item.h"
class CCandy :
	public CItem
{
public:
	CCandy();
	virtual ~CCandy();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	float		m_fForce;	// 가중치
};

#endif // !__CANDY_H__


