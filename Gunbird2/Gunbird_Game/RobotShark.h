#pragma once

#ifndef __ROBOTSHARK_H__
#define __ROBOTSHARK_H__

#include "Monster.h"
class CRobotShark :
	public CMonster
{
private:
	enum ATTACK { SHOT_1, SHOT_2, SHOT_3, SHOT_4, END };

public:
	CRobotShark();
	virtual ~CRobotShark();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fX, float _fY) { m_fDestPosX = _fX; m_fDestPosY = _fY; }
	void Set_OutPos(float _fX, float _fY) { m_fOutPosX = _fX; m_fOutPosY = _fY; }
	void Set_OutTime(DWORD _dwTime) { m_dwOutTime = _dwTime; }

private:
	bool Move(float _fDestX, float _fDestY);	// 이동
	bool MoveAngle(float _fDestX, float _fDestY);	// 이동
	void Frame_MoveEX();
	void Shot_1();
	void Shot_2();
	void Shot_3();

private:
	DWORD				m_dwOnTime;			// 배치된 시간
	DWORD				m_dwOutTime;		// 배치되고 퇴장하기까지의 시간
	float				m_fDestPosX;		// 바깥에서 화면 안으로 배치될 좌표 X
	float				m_fDestPosY;		// 바깥에서 화면 안으로 배치될 좌표 Y
	float				m_fOutPosX;			// 화면 밖으로 퇴장할 좌표 X
	float				m_fOutPosY;			// 화면 밖으로 퇴장할 좌표 Y
	DWORD				m_dwShotTime;		// 총알 쏘는 시간
	DWORD				m_dwShotDelay;		// 총알 쏘는 딜레이
	ATTACK				m_eAttackState;		// 공격 패턴
	FRAME				m_tBodyFrame;		// 몸통 프레임
	FRAME				m_tPersonFrame;		// 기체 조종사 프레임
	bool				m_bOnObject;		// 오브젝트 배치 플래그
	bool				m_bOutObject;		// 오브젝트 퇴장 플래그
};

#endif // !__ROBOTSHARK_H__


