// �����
// �÷��̾� ��ź ��
// ���� ��ź �� ����

#pragma once

#ifndef __UI_BOMB_H__
#define __UI_BOMB_H__

#include "Obj.h"
class CUI_Bomb final :
	public CObj
{
public:
	CUI_Bomb();
	virtual ~CUI_Bomb();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	int Get_Bomb() { return m_iBombCnt; }
	void Set_Bomb(int _iBombCnt) { m_iBombCnt = _iBombCnt; }
	void Add_Bomb(int _iBombCnt) { m_iBombCnt += _iBombCnt; }

private:
	int		m_iBombCnt;
};

#endif // !__UI_BOMB_H__


