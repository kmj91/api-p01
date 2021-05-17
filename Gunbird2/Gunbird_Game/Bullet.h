#pragma once
#include "Obj.h"

#ifndef __BULLET_H__
#define __BULLET_H__

class CBullet :
	public CObj
{
public:
	CBullet();
	virtual ~CBullet();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	float Get_Damage() { return m_fDamage; }
	void Set_Damage(float _fDamage) { m_fDamage = _fDamage; }
	void Set_BulletDelay(DWORD _dwDelay) { m_dwBulletDelay = _dwDelay; m_bOnBullet = false; }
	int Get_PlayerNum() { return m_iPlayerNum; }
	void Set_PlayerNum(int _iPlayerNum) { m_iPlayerNum = _iPlayerNum; }


	// �Ѿ��� �����ϰ� ���� ����
	void Change_Candy();

protected:
	DWORD m_dwBulletTime;	// ��ü ���� �ð�
	DWORD m_dwBulletDelay;	// ������
	float m_fDamage;		// �Ѿ� ������
	int m_iPlayerNum;		// �÷��̾� ��ȣ	(1P or 2P) - �÷��̾� �Ѿ˸� ���
	bool m_bOnBullet;		// �Ѿ� ���� �÷��� - �⺻ true, Set_BulletDelay() �Լ� ȣ��� false, Bullet_3�� ����ϴ� ��
};

#endif // !__BULLET_H__


