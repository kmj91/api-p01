// 기명준
// 아이템 인터페이스
// 아이템 종류를 구분하는 것과 점수 값을 멤버 변수로 가지고 있음

#pragma once

#ifndef __ITEM_H__
#define __ITEM_H__

#include "Obj.h"
class CItem abstract :
	public CObj
{
public:
	CItem();
	virtual ~CItem();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	ITEM::TYPE Get_ItemType() const { return m_enItemType; }
	int Get_Score() const { return m_iScore; }

protected:
	ITEM::TYPE		m_enItemType;		// 아이템 종류
	int				m_iScore;			// 점수
};

#endif // !__ITEM_H__


