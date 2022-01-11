// �����
// ���� �ΰ� ���� ����

#pragma once

#ifndef __REDBEANIE_H__
#define __REDBEANIE_H__

#include "Monster.h"
class CRedBeanie :
	public CMonster
{
private:
	enum STATE { IDLE, ATTACK, MOVE, END };

public:
	CRedBeanie();
	virtual ~CRedBeanie();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fY) { m_fDestPosY = _fY; }

private:
	void Scene_Change();	// ������ �� ���� ó��
	void Frame_MoveEX();	// ����Ʈ �̹��� ������ �̵�
	void Shot();			// ����

private:
	CRedBeanie::STATE		m_ePreState;
	CRedBeanie::STATE		m_eCurState;
	float					m_fDestPosY;		// �ٱ����� ȭ�� ������ ��ġ�� ��ǥ Y
	DWORD					m_dwShotTime;		// �Ѿ� ��� �ð�
	DWORD					m_dwShotDelay;		// �Ѿ� ��� ������
	int						m_iShot;			// �Ѿ�
	FRAME					m_tBodyFrame;		// �Ѿ� ���� �� ����Ʈ ���� ������� ����Ʈ ������
};

#endif // !__REDBEANIE_H__


