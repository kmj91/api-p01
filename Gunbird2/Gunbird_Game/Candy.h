// �����
// ��ź ���� �� �Ѿ��� ��ȯ�� ���� ������Ʈ
// ���������� �������ٰ� �� �ϴ����� �� ������
// �浹�ϸ� ���� ȹ��

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

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	float		m_fForce;	// ����ġ
};

#endif // !__CANDY_H__


