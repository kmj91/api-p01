#pragma once

#ifndef __CACTUSCANNON_H__
#define __CACTUSCANNON_H__

#include "Monster.h"
class CCactusCannon :
	public CMonster
{
private:
	enum STATE { HIDE, ON, DESTROY, END };

	enum ANGLE {
		A_90, A_95, A_105, A_120, A_135, A_150, A_165, A_175,
		A180, A175, A165, A150, A135, A120, A105, A95,
		A90, A85, A75, A60, A45, A30, A15, A5,
		A0, A_5, A_15, A_30, A_45, A_60, A_75, A_85,
		AEND
	};

public:
	CCactusCannon();
	virtual ~CCactusCannon();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fY) { m_fDestPosY = _fY; }

private:
	void Scene_Change();
	void Aim();				// Á¶ÁØ
	void Shot();

private:
	CCactusCannon::STATE	m_ePreState;
	CCactusCannon::STATE	m_eCurState;
	float					m_fDestPosY;
	DWORD					m_dwShotTime;		// ÃÑ¾Ë ½î´Â ½Ã°£
	DWORD					m_dwShotDelay;		// ÃÑ¾Ë ½î´Â µô·¹ÀÌ
	bool					m_bOnObject;		// ¿ÀºêÁ§Æ® ¹èÄ¡ ÇÃ·¡±×
};

#endif // !__CACTUSCANNON_H__


