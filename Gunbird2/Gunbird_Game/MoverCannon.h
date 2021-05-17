#pragma once

#ifndef __MOVERCANNON_H__
#define __MOVERCANNON_H__

#include "Monster.h"
class CMoverCannon :
	public CMonster
{
private:
	enum STATE { IDEL, ON, DESTROY, END };

	enum ANGLE { A_90, A_95, A_105, A_120, A_135, A_150, A_165, A_175, 
		A180, A175, A165, A150, A135, A120, A105, A95, 
		A90, A85, A75, A60, A45, A30, A15, A5, 
		A0, A_5, A_15, A_30, A_45, A_60, A_75, A_85,
		AEND };

public:
	CMoverCannon();
	virtual ~CMoverCannon();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fX, float _fY) { m_fDestPosX = _fX; m_fDestPosY = _fY; }

private:
	void Scene_Change();
	bool Move();			// �̵�
	void Aim();				// ����
	void Shot();

private:
	CMoverCannon::STATE			m_ePreState;
	CMoverCannon::STATE			m_eCurState;
	float						m_fDestPosX;		// �ٱ����� ȭ�� ������ ��ġ�� ��ǥ X
	float						m_fDestPosY;		// �ٱ����� ȭ�� ������ ��ġ�� ��ǥ Y
	DWORD						m_dwShotTime;		// �Ѿ� ��� �ð�
	DWORD						m_dwShotDelay;		// �Ѿ� ��� ������
};

#endif // !__MOVERCANNON_H__


