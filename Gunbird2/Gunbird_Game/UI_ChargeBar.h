// 기명준
// 플레이어 충전 바
// 충전량과 충전 레벨 관리

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

	// CObj을(를) 통해 상속됨
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
	int		m_iCharge;			// 8 단위... 같음 - 8값으로 이미지 1칸
	int		m_iChargeLevel;		// 39 이하 0레벨, 40 이상 119 이하 1레벨, 120 이상 239 이하 2레벨, 240 max 3레벨
};

#endif // !__UI_CHARGEBAR_H__


