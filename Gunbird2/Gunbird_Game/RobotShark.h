// �����
// �߰� ���� �κ� ��ũ ���� ����
// �ı��Ǹ� �Ŀ��� ������ ���

#pragma once

#ifndef __ROBOTSHARK_H__
#define __ROBOTSHARK_H__

#include "Monster.h"
class CRobotShark :
	public CMonster
{
private:
	enum ATTACK { SHOT_1, SHOT_2, SHOT_3, SHOT_4, END };

public:
	CRobotShark();
	virtual ~CRobotShark();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fX, float _fY) { m_fDestPosX = _fX; m_fDestPosY = _fY; }
	void Set_OutPos(float _fX, float _fY) { m_fOutPosX = _fX; m_fOutPosY = _fY; }

private:
	bool Move(float _fDestX, float _fDestY);		// �̵�
	bool MoveAngle(float _fDestX, float _fDestY);	// ��ȸ �̵�
	void Frame_MoveEX();		// ���� �̹��� ������ �̵�
	void Shot_1();				// ���� ���� 1
	void Shot_2();				// ���� ���� 2
	void Shot_3();				// ���� ���� 3

private:
	float				m_fDestPosX;		// �ٱ����� ȭ�� ������ ��ġ�� ��ǥ X
	float				m_fDestPosY;		// �ٱ����� ȭ�� ������ ��ġ�� ��ǥ Y
	float				m_fOutPosX;			// ȭ�� ������ ������ ��ǥ X
	float				m_fOutPosY;			// ȭ�� ������ ������ ��ǥ Y
	DWORD				m_dwShotTime;		// �Ѿ� ��� �ð�
	DWORD				m_dwShotDelay;		// �Ѿ� ��� ������
	ATTACK				m_eAttackState;		// ���� ����
	FRAME				m_tBodyFrame;		// ���� ������
	FRAME				m_tPersonFrame;		// ��ü ������ ������
	bool				m_bOnObject;		// ������Ʈ ��ġ �÷���
	bool				m_bOutObject;		// ������Ʈ ���� �÷���
};

#endif // !__ROBOTSHARK_H__


