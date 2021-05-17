#pragma once

#ifndef __FACTORY_H__
#define __FACTORY_H__

#include "Monster.h"
class CFactory :
	public CMonster
{
private:
	enum STATE { IDLE, OPEN, ATTACK, CLOSED, DESTROY, IDEL_END, END };

public:
	CFactory();
	virtual ~CFactory();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Scene_Change();

private:
	CFactory::STATE			m_ePreState;
	CFactory::STATE			m_eCurState;
	bool					m_bRemove;			// ªË¡¶
};

#endif // !__FACTORY_H__


