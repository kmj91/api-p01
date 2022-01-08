// �����
// �� ���� �Ķ��� �︮����
// ���� ���� ��ȸ�ϸ鼭 ���� �׸��� ���� �ð� �� ����

#pragma once

#ifndef __BLUE_H__
#define __BLUE_H__

#include "Monster.h"
class CBlue :
	public CMonster
{
private:
	enum STATE { FRONT, LEFT_1, LEFT_2, LEFT_3, RIGHT_1, RIGHT_2, RIGHT_3, END };

public:
	CBlue();
	virtual ~CBlue();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fX, float _fY) { m_fDestPosX = _fX; m_fDestPosY = _fY; }
	void Set_OutPos(float _fX, float _fY) { m_fOutPosX = _fX; m_fOutPosY = _fY; }
	void Set_OutTime(DWORD _dwTime) { m_dwOutTime = _dwTime; }
	void Set_DestAngle(float _fAngle) { m_fDestAngle = _fAngle; }
	void Set_OutAngle(float _fAngle) { m_fOutAngle = _fAngle; }

private:
	void Scene_Change();
	bool Move(float _fDestX, float _fDestY, float _fAngle);		// �̵�
	void Shot();

private:
	CBlue::STATE		m_ePreState;
	CBlue::STATE		m_eCurState;
	DWORD				m_dwOnTime;			// ��ġ�� �ð�
	DWORD				m_dwOutTime;		// ��ġ�ǰ� �����ϱ������ �ð�
	float				m_fDestPosX;		// �ٱ����� ȭ�� ������ ��ġ�� ��ǥ X
	float				m_fDestPosY;		// �ٱ����� ȭ�� ������ ��ġ�� ��ǥ Y
	float				m_fOutPosX;			// ȭ�� ������ ������ ��ǥ X
	float				m_fOutPosY;			// ȭ�� ������ ������ ��ǥ Y
	float				m_fDestAngle;		// ��ġ ��ȸ ����
	float				m_fOutAngle;		// ���� ��ȸ ����
	int					m_iShot;			// �Ѿ�
	bool				m_bOnObject;		// ������Ʈ ��ġ �÷���
	bool				m_bOutObject;		// ������Ʈ ���� �÷���
};

#endif // !__BLUE_H__


