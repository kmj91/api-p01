// �����
// �÷��̾� ������
// ��� �ִϸ��̼� �������� �̹��� ���̰� �޶�
// ���°� ����� �� ��ġ ��ǥ�� �ʱ�ȭ�� �� �����ؾ� ��

#pragma once

#ifndef __MARION_H__
#define __MARION_H__

#include "Player.h"
class CMarion final :
	public CPlayer
{
public:
	CMarion();
	virtual ~CMarion();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update();
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	// �Ѿ� �Ŀ� ��
	virtual void PowerUp() override;
	// �Ѿ� �Ŀ� �ٿ�
	virtual void PowerDown() override;
	// Ű �Է�
	// _Key : �Էµ� Ű enum ��
	virtual void Key_Input(int _Key);
	// ĳ���� �̵�
	virtual void Move_LL();
	virtual void Move_LU();
	virtual void Move_LD();
	virtual void Move_RR();
	virtual void Move_RU();
	virtual void Move_RD();
	virtual void Move_UU();
	virtual void Move_DD();
	// �̵� ��
	virtual void Move_End();
	// �Ϲ� ����
	virtual void Attack();
	// �Ϲ� ���� ��
	virtual void Attack_End();
	// ��ź ����
	virtual void Bomb();
	// ���� ����
	virtual void Melee();
	// �ٽ� �ϱ�
	virtual void Continue();

private:
	void Scene_Change();			// ������ �� ���� ó��
	void Move_Change();				// �̵� ���� ���� ó��
	void Shot();					// �Ϲ� ���� ó��
	void Create_Effect();			// �� ����Ʈ ����
	bool CanKeyInput();				// Ű ó���� �������� �˻�
	bool CanAttack();				// ������ �� �ִ��� �˻�

	template <typename T>
	CObj* Create_Bullet(float _fX, float _fY, float _fAngle)
	{
		return CAbstractFactory<T>::Create(_fX, _fY, _fAngle);
	}

private:
	DWORD		m_dwEffectCreateTime;	// ����Ʈ ���� �ð�
	DWORD		m_dwEffectCreateDelay;	// ����Ʈ ���� ������
	bool		m_bBombEffect;			// ��ź ����Ʈ�� ���� - �����ư��鼭 ��ġ ����
	bool		m_bMeleeHit;			// ���� ���� ���� Ȯ�ο�
	BULLET		m_tSubBulletCnt;		// ���� ���� �̻��Ͽ� ����ü
};

#endif // !__MARION_H__

