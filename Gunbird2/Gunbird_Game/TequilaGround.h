// �����
// 1 ������ ����
// �ı��� �� 2 ������ ������ ����

#pragma once

#ifndef __TEQUILAGROUND_H__
#define __TEQUILAGROUND_H__

#include "Boss.h"
class CTequilaGround :
	public CBoss
{
private:
	enum class STATE { WAIT, CLOSE_DOOR, PAVED_ROAD,
		IDLE,
		ATTACK,
		DESTROY,
		FINAL_1,
		FINAL_2,
		FINAL_3,
		FINAL_4,
		FINAL_5,
		END };

	enum class SPRITE { L_CANNON, R_CANNON, LL_CANNON, RR_CANNON, CENTER_CANNON, END};
	enum class PATTERN { IDLE, L_SUB, R_SUB, LL_SUB, RR_SUB, FRONT, BACK, END };

public:
	CTequilaGround();
	virtual ~CTequilaGround();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	// CBoss��(��) ���� ��ӵ�
	virtual void Check_Damage(int _iPlayerNum) override;	// ���� ������ �ı� ������ üũ

private:
	void Scene_Change();		// ������ �� ���� ó��
	void Frame_MoveEX();		// �κ� ��������Ʈ ĳ�� ������ ������Ʈ
	void Action();				// ���� �ൿ ����
	void Move();				// ���� �̵�
	void DamageEffect();		// �ı� ����Ʈ
	void L_SideSub_Shot();		// ���� ���� ĳ��
	void R_SideSub_Shot();		// ������ ���� ĳ��
	void L_SideSub_Buckshot();	// ���� ���� ĳ�� ��ź
	void R_SideSub_Buckshot();	// ������ ���� ĳ�� ��ź
	void Front_Shot();			// ���� ĳ��
	void L_FrontSub_Shot();		// ���� ���� ���� ĳ��
	void R_FrontSub_Shot();		// ������ ���� ���� ĳ��
	void Back_AutoShot();		// ���� ���� ����
	void Back_Buckshot();		// ���� ��ź ����
	void Destroy_Shot(float _fAngle);	// �ı��� �� ����

private:
	STATE		m_ePreState;		// ���� ����
	STATE		m_eCurState;		// ��ä�� ����
	float		m_fShotAngle;		// �Ѿ� ����
	DWORD		m_dwShotTime;		// �ð�
	DWORD		m_dwShotDelay;		// ������
	DWORD		m_dwDestroyTime;	// �ı� ����Ʈ �ð�
	DWORD		m_dwDestroyDelay;	// �ı� ����Ʈ ������
	DWORD		m_dwMoveTime;		// �̵� �ð�
	DWORD		m_dwMoveDelay;		// �̵� ������
	DELAY		m_tPatternDelay[static_cast<UINT>(PATTERN::END)];	// ���� ������
	FRAME		m_tCannonFrameArr[static_cast<UINT>(SPRITE::END)];	// �κ� ��������Ʈ ĳ���� ������
	DAMAGE		m_tDamageFlagArr[static_cast<UINT>(SPRITE::END)];	// �κ� �������� ������ �÷���
	bool		m_bBackAttack;		// ���� ���� ���� �����ư��鼭 ����ϱ� ���� �÷���
	bool		m_bMoveFlag;		// �̵� �÷��� - �¿� �̵��ϴµ� �����ð� ����� �ݴ�������� �̵���
};

#endif // !__TEQUILAGROUND_H__


