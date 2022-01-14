// �����
// ����Ʈ �������̽�
// ����Ʈ ���� �ð��� �����̸� �̿��ؼ� �����ؼ� ����Ʈ�� �߻��ϰ� �ϱ⵵ ��
// ��� ��ǥ�� ������ Ÿ�� + �����ǥ�� ���� ��ġ ��ǥ�� �����ϱ⵵ ��

#pragma once

#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "Obj.h"
class CEffect abstract:
	public CObj
{
public:
	CEffect();
	virtual ~CEffect();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_EffectDelay(DWORD _dwDelay) { m_dwEffectDelay = _dwDelay; }
	void Set_RelativePos(float _fX, float _fY) { m_fRelativePosX = _fX; m_fRelativePosY = _fY; }

protected:
	DWORD		m_dwEffectTime;			// ����Ʈ ��ü ���� �ð�
	DWORD		m_dwEffectDelay;		// ����Ʈ ������
	float		m_fRelativePosX;		// �����ǥ X
	float		m_fRelativePosY;		// �����ǥ Y
	bool		m_bOnEffect;			// ����Ʈ ���� �÷���
};

#endif // !__EFFECT_H__


