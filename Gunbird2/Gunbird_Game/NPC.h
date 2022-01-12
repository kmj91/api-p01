// �����
// NPC �θ� �������̽�
// NPC���� �̵��ϰų� �������ְų� �� ��� ���

#pragma once

#ifndef __NPC_H__
#define __NPC_H__

#include "Obj.h"
class CNPC abstract :
	public CObj
{
public:
	CNPC();
	virtual ~CNPC();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fX, float _fY) { m_fDestPosX = _fX; m_fDestPosY = _fY; }
	// ���� ������ �ƴ� �̴� ȭ����� ������ ���� ��Ʈ���ϱ� ���� �ϱ� ������
	void Move_DestPos(float _fX, float _fY) { m_fDestPosX = m_tInfo.fX + _fX; m_fDestPosY = m_tInfo.fY + _fY; }

protected:
	float		m_fDestPosX;		// �̵� ��ǥ X	(�ܺο��� �ش� ��ǥ�� ���� �޴´�)
	float		m_fDestPosY;		// �̵� ��ǥ Y
};

#endif // !__NPC_H__


