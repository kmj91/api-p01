// �����
// UI �÷��̾� ������
// �÷��̾��� ���� �������� ǥ��

#pragma once

#ifndef __UI_PLAYERLIFE_H__
#define __UI_PLAYERLIFE_H__

#include "Obj.h"
class CUI_PlayerLife final :
	public CObj
{
public:
	CUI_PlayerLife();
	virtual ~CUI_PlayerLife();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	int Get_Life() { return m_iLife; }

	void Add_Life(int _iLife) { m_iLife += _iLife; }

private:
	int		m_iLife;
};

#endif // !__UI_PLAYERLIFE_H__


