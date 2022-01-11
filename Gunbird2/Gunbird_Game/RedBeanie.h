// 기명준
// 빨간 두건 공중 몬스터

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
	void Scene_Change();	// 프레임 씬 변경 처리
	void Frame_MoveEX();	// 이펙트 이미지 프레임 이동
	void Shot();			// 공격

private:
	CRedBeanie::STATE		m_ePreState;
	CRedBeanie::STATE		m_eCurState;
	float					m_fDestPosY;		// 바깥에서 화면 안으로 배치될 좌표 Y
	DWORD					m_dwShotTime;		// 총알 쏘는 시간
	DWORD					m_dwShotDelay;		// 총알 쏘는 딜레이
	int						m_iShot;			// 총알
	FRAME					m_tBodyFrame;		// 총알 공격 때 이펙트 몸통 기모으는 이펙트 프레임
};

#endif // !__REDBEANIE_H__


