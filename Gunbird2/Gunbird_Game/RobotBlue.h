// 기명준
// 이족 보행 파란색 로봇
// 이동할 때 다른 오브젝트 처럼 똑같은 속도로 움직이면
// 굉장히 어색함 마치 런닝 머신위를 걷는 것 같은 느낌을 줌
// 프레임 구간별로 이동할 좌표 속도 값을 따로 정해줌

#pragma once

#ifndef __ROBOTBLUE_H__
#define __ROBOTBLUE_H__

#include "Monster.h"
class CRobotBlue :
	public CMonster
{
private:
	enum STATE { IDLE_R, IDLE_L, MOVE_R, MOVE_L, DESTROY,END };
	enum AIM { A40, A43, A46, A49, A52, A55, A58, A61, A64, A67,
		A70, A73, A76, A79, A82, A85, A88, A91, A94, A97,
		A100, A103, A106, A109, A112, A115, A118, A121, A124, A127,
		A130, A133, A136, AEND};

public:
	CRobotBlue();
	virtual ~CRobotBlue();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Scene_Change();		// 프레임 씬 변경 처리
	void Frame_MoveEX();		// 뚜껑 이미지 프레임 이동
	void Shot_1(float _fAngle);	// 전방위 공격
	void Shot_2();				// 공격
	void Aim();					// 조준

	bool Move_RU();				// 이동

private:
	CRobotBlue::STATE		m_ePreState;
	CRobotBlue::STATE		m_eCurState;
	int						m_iOldFrame;		// 이전 프레임 저장 값
	int						m_iActionCnt;		// 패턴 행동 카운트 값
	DWORD					m_dwShotTime;		// 총알 쏘는 시간
	DWORD					m_dwShotDelay;		// 총알 쏘는 딜레이
	FRAME					m_tTopFrame;		// 뚜껑 프레임
	bool					m_bOnObject;
};

#endif // !__ROBOTBLUE_H__
