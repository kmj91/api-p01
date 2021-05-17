#pragma once

#ifndef __SECTOR_H__
#define __SECTOR_H__

#include "Obj.h"
class CSector :
	public CObj
{
public:
	CSector();
	virtual ~CSector();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fX, float _fY) { m_fDestPosX = _fX; m_fDestPosY = _fY; }

	float Get_DestPosX() { return m_fDestPosX; }
	float Get_DestPosY() { return m_fDestPosY; }

private:
	float					m_fDestPosX;		// ÀÌµ¿ ÁÂÇ¥ X
	float					m_fDestPosY;		// ÀÌµ¿ ÁÂÇ¥ Y
};

#endif // !__SECTOR_H__


