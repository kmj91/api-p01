// 기명준
// 마리온 폭탄 이펙트
// 캐릭터 중심으로 회전 이동 함

#pragma once

#ifndef __MARIONBOMBEFFECT_H__
#define __MARIONBOMBEFFECT_H__

#include "Effect.h"
class CMarionBombEffect :
	public CEffect
{
private:
	enum ANGLE {
		A90, A70, A45, A20,
		A0, A_20, A_45, A_70,
		A_90, A_110, A_135, A_160,
		A180, A160, A135, A110,
		END
	};

public:
	CMarionBombEffect();
	virtual ~CMarionBombEffect();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fDestX, float _fDestY) { m_fDestX = _fDestX; m_fDestY = _fDestY; }

private:
	bool Move();		// 회전 이동
	void Angle();		// 각도에 따른 이미지 변경

private:
	float m_fDestX;					// 이동 좌표 X
	float m_fDestY;					// 이동 좌표 Y
	DWORD m_dwEffectCreateTime;		// 이펙트 생성 시간
	DWORD m_dwEffectCreateDelay;	// 이펙트 생성 딜레이
};

#endif // !__MARIONBOMBEFFECT_H__


