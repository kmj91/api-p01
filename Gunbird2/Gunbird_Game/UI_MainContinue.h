/**************************************************

���� ��Ƽ��
1Player 2Player ��� ����Ͽ��� ��

***************************************************/

#pragma once

#ifndef __UI_MAINCONTINUE_H__
#define __UI_MAINCONTINUE_H__

#include "Obj.h"

class CUI_MainContinue final :
	public CObj
{
public:
	CUI_MainContinue();
	virtual ~CUI_MainContinue();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	// ���� ��Ƽ�� ī��Ʈ ����
	void Start_Countdown();
	// ���� ��Ƽ�� ī��Ʈ ����
	void Stop_Countdown();
	// ī��Ʈ ����
	void NextCount();

private:
	// ��Ƽ�� �ð� ����
	void Update_Count();

private:
	// UI�� ȭ�鿡 ������� �����ϴ� �÷���
	bool m_bOnUI;
};

#endif // !__UI_MAINCONTINUE_H__


