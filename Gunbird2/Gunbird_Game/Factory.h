// 기명준
// 지상 공장
// 파괴되면 폭탄 아이템 드랍

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
	void Scene_Change();	// 프레임 씬 변경 처리

private:
	CFactory::STATE			m_ePreState;
	CFactory::STATE			m_eCurState;
	bool					m_bRemove;			// 삭제
};

#endif // !__FACTORY_H__


