#pragma once

#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "Obj.h"
class CEffect :
	public CObj
{
public:
	CEffect();
	virtual ~CEffect();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_EffectDelay(DWORD _dwDelay) { m_dwEffectDelay = _dwDelay; }
	void Set_RelativePos(float _fX, float _fY) { m_fRelativePosX = _fX; m_fRelativePosY = _fY; }

protected:
	DWORD		m_dwEffectTime;			// 이펙트 객체 생성 시간
	DWORD		m_dwEffectDelay;		// 이펙트 딜레이
	float		m_fRelativePosX;		// 상대좌표 X
	float		m_fRelativePosY;		// 상대좌표 Y
	bool		m_bOnEffect;			// 이펙트 제어 플래그
};

#endif // !__EFFECT_H__


