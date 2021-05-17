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

	void Key_Input(int _Key);
	PLAYER::NAME Get_State();

private:
	void Scene_Change();

private:
	PLAYER::NAME		m_ePreState;
	PLAYER::NAME		m_eCurState;
};

#endif // !__SELECTPLAYER_H__


