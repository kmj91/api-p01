// 기명준
// 적이 파괴된 후 드랍하는 코인 아이템
// 맵 스크롤과 같은 속도로 이동 함 (지상 오브젝트 취급)
// 충돌하면 점수 획득

#pragma once

#ifndef __COIN_H__
#define __COIN_H__

#include "item.h"
class CCoin :
	public CItem
{
public:
	CCoin();
	virtual ~CCoin();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__COIN_H__


