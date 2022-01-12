// 기명준
// 이동하는 고정 오브젝트
// 배경의 일부이거나 높이를 표현하기 위해 덮어씌우는 오브젝트

#pragma once

#ifndef __MOVESTATIC_H__
#define __MOVESTATIC_H__

#include "Obj.h"
class CMoveStatic :
	public CObj
{
public:
	CMoveStatic();
	virtual ~CMoveStatic();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fX, float _fY) { m_fDestPosX = _fX; m_fDestPosY = _fY; }

private:
	void Update_RectEx();	// 이미지 RECT 정보 갱신

private:
	float			m_fDestPosX;
	float			m_fDestPosY;
};

#endif // !__MOVESTATIC_H__


