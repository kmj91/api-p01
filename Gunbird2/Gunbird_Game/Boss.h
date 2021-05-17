#pragma once

#ifndef __BOSS_H__
#define __BOSS_H__

#include "Monster.h"

class CBoss abstract :
	public CMonster
{
public:
	explicit CBoss();
	virtual ~CBoss();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	virtual void Check_Damage(int _iPlayerNum) = 0;	// ���� ������ �ı� ������ üũ

public:
	void Update_Rect();		// ������ �������� ��Ʈ�� ������ ���� Obj �Ÿ� ȣ�� ���� �ʰ� �̰� �Ѵ�				
	bool Check_Hit(const RECT* _stpBullet, float _fDamage, int _iPlayerNum);	// �Ϲ� ���ݿ����� ���� - ������ ��Ʈ�� �˻��ؼ� �κ� ���ظ� �޴´�
	void All_Hit(float _fDamage, int _iPlayerNum);	// ��ź���� ��ü ���ݿ� ���� ����

protected:
	vector<RECT>		m_vecHitRectPos;	// ������Ʈ �̹��� ���� ��Ʈ�ڽ� ��ġ
	vector<RECT>		m_vecHitRect;		// ������Ʈ ��ġ ���� ������ ��Ʈ�ڽ�
	vector<float>		m_vecRectHp;		// ������ ��Ʈ �κ� ü��
	bool				m_bDestroy;			// �ı� �÷���
	bool				m_bAttack;			// ���� �÷���
};

#endif // !__BOSS_H__


