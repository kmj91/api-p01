// �����
// ������ �������̽�
// ������ ������ �����ϴ� �Ͱ� ���� ���� ��� ������ ������ ����

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

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	ITEM::TYPE Get_ItemType() const { return m_enItemType; }
	int Get_Score() const { return m_iScore; }

protected:
	ITEM::TYPE		m_enItemType;		// ������ ����
	int				m_iScore;			// ����
};

#endif // !__ITEM_H__


