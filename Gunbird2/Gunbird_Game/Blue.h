// 기명준
// 적 몬스터 파란색 헬리콥터
// 공중 몬스터 선회하면서 등장 그리고 일정 시간 후 퇴장

#pragma once

#ifndef __BLUE_H__
#define __BLUE_H__

#include "Monster.h"
class CBlue :
	public CMonster
{
private:
	enum STATE { FRONT, LEFT_1, LEFT_2, LEFT_3, RIGHT_1, RIGHT_2, RIGHT_3, END };

public:
	CBlue();
	virtual ~CBlue();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fX, float _fY) { m_fDestPosX = _fX; m_fDestPosY = _fY; }
	void Set_OutPos(float _fX, float _fY) { m_fOutPosX = _fX; m_fOutPosY = _fY; }
	void Set_OutTime(DWORD _dwTime) { m_dwOutTime = _dwTime; }
	void Set_DestAngle(float _fAngle) { m_fDestAngle = _fAngle; }
	void Set_OutAngle(float _fAngle) { m_fOutAngle = _fAngle; }

private:
	void Scene_Change();
	bool Move(float _fDestX, float _fDestY, float _fAngle);		// 이동
	void Shot();

private:
	CBlue::STATE		m_ePreState;
	CBlue::STATE		m_eCurState;
	DWORD				m_dwOnTime;			// 배치된 시간
	DWORD				m_dwOutTime;		// 배치되고 퇴장하기까지의 시간
	float				m_fDestPosX;		// 바깥에서 화면 안으로 배치될 좌표 X
	float				m_fDestPosY;		// 바깥에서 화면 안으로 배치될 좌표 Y
	float				m_fOutPosX;			// 화면 밖으로 퇴장할 좌표 X
	float				m_fOutPosY;			// 화면 밖으로 퇴장할 좌표 Y
	float				m_fDestAngle;		// 배치 선회 각도
	float				m_fOutAngle;		// 퇴장 선회 각도
	int					m_iShot;			// 총알
	bool				m_bOnObject;		// 오브젝트 배치 플래그
	bool				m_bOutObject;		// 오브젝트 퇴장 플래그
};

#endif // !__BLUE_H__


