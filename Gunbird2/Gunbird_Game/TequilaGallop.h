#pragma once

#ifndef __TEQUILAGALLOP_H__
#define __TEQUILAGALLOP_H__

#include "Boss.h"
class CTequilaGallop :
	public CBoss
{
private:
	enum class STATE { WAIT, PHASE_MOVE, PHASE_CHANGE,
		READY,
		NEXT_PATTERN,		// ���� ���� Ȯ��
		L_ARM_ATTACK,		// ���� ����
		R_ARM_ATTACK,		// ������ ����
		FRONT_ATTACK,		// ���� ����
		BACK_ATTACK,		// �� ����
		SHOULDER_ATTACK,	// ��� ����
		DESTROY,
		FINAL_1,
		FINAL_2,
		FINAL_3,
		FINAL_4,
		FINAL_5,
		END };

	enum class SPRITE { BODY, L_ARM, R_ARM, END};

public:
	CTequilaGallop();
	virtual ~CTequilaGallop();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	// CBoss��(��) ���� ��ӵ�
	virtual void Check_Damage(int _iPlayerNum) override;	// ���� ������ �ı� ������ üũ

private:
	void Scene_Change();
	void Frame_MoveEX();	// �κ� ��������Ʈ ������ ������Ʈ
	void Action();			// ���� �ൿ
	void Move();			// �̵�
	void DamageEffect();	// �ı� ����Ʈ
	void Shake_Body();		// ��ü ����
	void Pipe_Smoke();		// ��ⱸ ����Ʈ ���
	void L_Arm_Shot();		// ���� ����
	void R_Arm_Shot();		// ������ ����
	void Front_Shot(float _fX, float _fY);		// ���� ����
	void Back_Shot(float _fX, float _fY);		// �� ����
	void Shoulder_Shot();	// ��� ����
	void Shot_5(float _fAngle);

private:
	STATE			m_ePreState;		// ���� ����
	STATE			m_eCurState;		// ��ü�� ����
	DWORD			m_dwShotCnt;		// �Ѿ� ��� Ƚ��
	DWORD			m_dwShotTime;		// �Ѿ� ��� �ð�
	DWORD			m_dwShotDelay;		// �Ѿ� ��� ������
	float			m_fShotAngle;		// �Ѿ� ��� ����
	float			m_fBodyPosY;		// ��ü Y ��ǥ - m_tInfo ��ǥ + �ش� ��ǥ
	float			m_fArmPosY;			// �� Y ��ǥ - m_tInfo ��ǥ + �ش� ��ǥ
	DWORD			m_dwShakeTime;		// ���� �ð�
	DWORD			m_dwShakeDelay;		// ���� ������
	DWORD			m_dwMoveTime;		// �̵� �ð�
	DWORD			m_dwMoveDelay;		// �̵� ������
	DWORD			m_dwUnderFrameCnt;	// ��ü ������ ī��Ʈ - ����� �ֱ�� ���� ���� �ŷ��ߵ�
	DWORD			m_dwWhellSmokeTime;	// ���� ���� �ð�
	DWORD			m_dwWhellSmokeDelay;// ���� ���� ������
	DWORD			m_dwChargeTime;		// ���� ���� �ð�
	DWORD			m_dwChargeDelay;	// ���� ���� ������
	DWORD			m_dwSoundTime;		// �Ҹ� �ð�
	DWORD			m_dwSoundDelay;		// �Ҹ� ������
	DWORD			m_dwPatternIndex;	// ���� �ε���
	vector<STATE>	m_vecPattern;		// ������ �����ϴ� ����
	FRAME			m_tSubFrameArr[static_cast<UINT>(SPRITE::END)];		// �κ� ��������Ʈ �� ������
	DAMAGE			m_bDamageFlagArr[static_cast<UINT>(SPRITE::END)];	// �κ� �������� ������ �÷���
	bool			m_bMoveFlag;		// �̵� �÷��� - �¿� �̵��ϴµ� �����ð� ����� �ݴ�������� �̵���
	bool			m_bDamageLeftMove;	// ���� �̵�
	bool			m_bDamageRightMove;	// ������ �̵�
};

#endif // !__TEQUILAGALLOP_H__


