#pragma once

#ifndef __REDBEANIE_H__
#define __REDBEANIE_H__

#include "Monster.h"
class CRedBeanie :
	public CMonster
{
private:
	enum STATE { IDLE, ATTACK, MOVE, END };

public:
	CRedBeanie();
	virtual ~CRedBeanie();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fY) { m_fDestPosY = _fY; }

private:
	void Scene_Change();
	void Frame_MoveEX();
	void Shot();

private:
	CRedBeanie::STATE		m_ePreState;
	CRedBeanie::STATE		m_eCurState;
	float					m_fDestPosY;		// ¹Ù±ù¿¡¼­ È­¸é ¾ÈÀ¸·Î ¹èÄ¡µÉ ÁÂÇ¥ Y
	DWORD					m_dwShotTime;		// ÃÑ¾Ë ½î´Â ½Ã°£
	DWORD					m_dwShotDelay;		// ÃÑ¾Ë ½î´Â µô·¹ÀÌ
	int						m_iShot;			// ÃÑ¾Ë
	FRAME					m_tBodyFrame;
};

#endif // !__REDBEANIE_H__


