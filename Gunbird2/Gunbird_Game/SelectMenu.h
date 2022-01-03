// �����
// ĳ���� ���� ��
// ĳ���� ���� ȭ��

#pragma once

#ifndef __SELECTMENU_H__
#define __SELECTMENU_H__

#include "Scene.h"
class CSelectMenu :
	public CScene
{
public:
	CSelectMenu();
	virtual ~CSelectMenu();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Key_Check();
	void Frame_Move();

private:
	CObj*		m_pSelectFocus;					// �÷��̾� ���� ������Ʈ
	FRAME		m_tFrame[PLAYER::NAME_END];		// �÷��̾� �̹��� ������ ����
};

#endif // !__SELECTMENU_H__


