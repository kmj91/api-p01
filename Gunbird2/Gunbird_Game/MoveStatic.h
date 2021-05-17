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
	void Update_RectEx();

private:
	float			m_fDestPosX;
	float			m_fDestPosY;
};

#endif // !__MOVESTATIC_H__


