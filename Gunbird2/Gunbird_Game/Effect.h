// 기명준
// 이펙트 인터페이스
// 이펙트 생성 시간과 딜레이를 이용해서 지연해서 이펙트를 발생하게 하기도 함
// 상대 좌표를 가지고 타겟 + 상대좌표를 통해 위치 좌표를 생성하기도 함

#pragma once

#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "Obj.h"
class CEffect abstract:
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


