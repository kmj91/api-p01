// �����
// �÷��̾� ���� ��
// �������� ���� ���� ����

#pragma once

#ifndef __UI_CHARGEBAR_H__
#define __UI_CHARGEBAR_H__

#include "Obj.h"
class CUI_ChargeBar final :
	public CObj
{
public:
	CUI_ChargeBar();
	virtual ~CUI_ChargeBar();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	int Get_Charge() { return m_iCharge; }
	void Set_Charge(int _iCharge) { m_iCharge = _iCharge; }
	void Add_Charge(int _iCharge) { m_iCharge += _iCharge; }
	int Get_ChargeLevel() { return m_iChargeLevel; }
	void Set_ChargeLevel(int _iChargeLevel) { m_iChargeLevel = _iChargeLevel; }
	void Add_ChargeLevel(int _iChargeLevel) { m_iChargeLevel += _iChargeLevel; }

private:
	int		m_iCharge;			// 8 ����... ���� - 8������ �̹��� 1ĭ
	int		m_iChargeLevel;		// 39 ���� 0����, 40 �̻� 119 ���� 1����, 120 �̻� 239 ���� 2����, 240 max 3����
};

#endif // !__UI_CHARGEBAR_H__


