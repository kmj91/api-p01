// �����
// ĳ���� ���� ǥ�� ������Ʈ
// ���� ��Ŀ�� ���� �� ȭ�鿡�� ������ ĳ���� ǥ��

#pragma once

#ifndef __SELECTPLAYER_H__
#define __SELECTPLAYER_H__

#include "Obj.h"
class CPlayerSelect :
	public CObj
{
public:
	CPlayerSelect();
	virtual ~CPlayerSelect();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Key_Input(int _Key);			// Ű �Է� ó��
	PLAYER::NAME Get_State();			// ���� ������ ĳ���� ��ȣ ���

private:
	void CharacterChange();				// ĳ���� ��ü

private:
	PLAYER::NAME		m_ePreCharacter;	// ���� ĳ���� ��ȣ
	PLAYER::NAME		m_eCurCharacter;	// ��ü�� ĳ���� ��ȣ
};

#endif // !__SELECTPLAYER_H__


