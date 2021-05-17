#pragma once

#ifndef __ROBOTBLUE_H__
#define __ROBOTBLUE_H__

#include "Monster.h"
class CRobotBlue :
	public CMonster
{
private:
	enum STATE { IDLE_R, IDLE_L, MOVE_R, MOVE_L, DESTROY,END };
	enum AIM { A40, A43, A46, A49, A52, A55, A58, A61, A64, A67,
		A70, A73, A76, A79, A82, A85, A88, A91, A94, A97,
		A100, A103, A106, A109, A112, A115, A118, A121, A124, A127,
		A130, A133, A136, AEND};

public:
	CRobotBlue();
	virtual ~CRobotBlue();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Scene_Change();
	void Frame_MoveEX();
	void Shot_1(float _fAngle);
	void Shot_2();
	void Aim();				// Á¶ÁØ

	bool Move_RU();			// ÀÌµ¿

private:
	CRobotBlue::STATE		m_ePreState;
	CRobotBlue::STATE		m_eCurState;
	int						m_iOldFrame;
	int						m_iActionCnt;
	DWORD					m_dwShotTime;		// ÃÑ¾Ë ½î´Â ½Ã°£
	DWORD					m_dwShotDelay;		// ÃÑ¾Ë ½î´Â µô·¹ÀÌ
	FRAME					m_tTopFrame;		// ¶Ñ²± ÇÁ·¹ÀÓ
	bool					m_bOnObject;
};

#endif // !__ROBOTBLUE_H__
