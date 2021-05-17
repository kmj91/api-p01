#pragma once

#ifndef __ITEM_H__
#define __ITEM_H__

#include "Obj.h"
class CItem :
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


