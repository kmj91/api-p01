// �����
// �÷��̾� ĳ���� �θ� Ŭ����
// ���� ó�� �Լ��� ��� ������ ����

#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Obj.h"
class CPlayer abstract :
	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	// �Ѿ� �Ŀ� ��
	virtual void PowerUp() {}
	// �Ѿ� �Ŀ� �ٿ�
	virtual void PowerDown(){}
	// Ű �Է�
	// _Key : �Էµ� Ű enum ��
	virtual void Key_Input(int _Key) {}
	// ĳ���� �̵�
	virtual void Move_LL() = 0;
	virtual void Move_LU() = 0;
	virtual void Move_LD() = 0;
	virtual void Move_RR() = 0;
	virtual void Move_RU() = 0;
	virtual void Move_RD() = 0;
	virtual void Move_UU() = 0;
	virtual void Move_DD() = 0;
	virtual void Move_End() = 0;
	// �Ϲ� ����
	virtual void Attack() = 0;
	virtual void Attack_End() = 0;
	// ��ź ����
	virtual void Bomb() = 0;
	// ���� ����
	virtual void Melee() = 0;

public:
	// �÷��̾� ��ȣ ��ȯ
	int Get_PlayerNum() { return m_iPlayerNum; }
	// �÷��̾� 1p, 2p ����
	// _iNum : �Ķ���� ���� 1�� ��� 1�÷��̾�, 2�� 2�÷��̾�
	void Set_PlayerNum(int _iNum) { m_iPlayerNum = _iNum; }

protected:
	PLAYER::STATE		m_ePreState;
	PLAYER::STATE		m_eCurState;
	RECT				m_tMeleeRect;			// ���� ���� ��Ʈ
	DWORD				m_dwKeyTime;			// Ű �Է� �ð�
	DWORD				m_dwKeyDelay;			// Ű �Է� ������
	DWORD				m_dwBulletTime;			// �Ѿ� �߽� �ð�
	DWORD				m_dwBulletDelay;		// �Ѿ� �߽� ������
	DWORD				m_dwRespawnTime;		// ������ �ð�
	DWORD				m_dwRespawnDelay;		// ������ ������
	DWORD				m_dwHpLockTime;			// ���� �ð�
	DWORD				m_dwHpLockDelay;		// ���� ������
	DWORD				m_dwChargeTime;			// ���� ���� Ű �Է� �ð�
	DWORD				m_dwChargeDelay;		// ���� ���� Ű �Է� ������
	DWORD				m_dwChargeBulletTime;	// ���� ���� �ð�
	DWORD				m_dwChargeBulletDelay;	// ���� ���� ������
	DWORD				m_dwPowerDownTime;		// �Ŀ� �ٿ� �ð�
	DWORD				m_dwPowerDownDelay;		// �Ŀ� �ٿ� ������
	int					m_iPlayerNum;			// �÷��̾� ��ȣ	(1P or 2P)
	int					m_iShotPower;			// �Ѿ� �Ŀ�
	int					m_iShotCnt;				// �Ѿ� ����
	int					m_iChargeBulletCnt;		// ������ ����
	float				m_fAngelY;				// �÷��̾� �¿� �̵� �� ���� ���� ��
	float				m_fMaxRotAngle;			// �ִ� ��ȸ ����
	float				m_fRotSpeed;			// ȸ�� �ӵ�
	bool				m_bRespawn;				// ������ ��
	bool				m_bOnShot;				// ���� ��ư ������ �ִ°�?
	bool				m_bImmutable;			// ��������Ʈ ���� - �ٸ� Ű �����̳� �������� ��������Ʈ�� ��ü�Ǵ°� ���� �÷���
};

#endif // !__PLAYER_H__


