// 기명준
// 보스전 텍스트 박스
// 대사 출력 후 보스전 진행
// 대사는 이미지 파일임

#pragma once

#ifndef __STAGE1_BOSSTEXTBOX_H__
#define __STAGE1_BOSSTEXTBOX_H__

#include "Obj.h"
class CStage1_BossTextBox :
	public CObj
{
private:
	enum STATE {ON_TEXTBOX, ACTION_1, ACTION_2, MOVE_BlADE, MOVE_SHARK, BOSS_GO, END };

public:
	CStage1_BossTextBox();
	virtual ~CStage1_BossTextBox();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Update_RectEX();
	void Scene_Change();		// 프레임 씬 변경 처리

private:
	CStage1_BossTextBox::STATE		m_ePreState;
	CStage1_BossTextBox::STATE		m_eCurState;
	DWORD							m_dwTime;
	DWORD							m_dwDelay;
	DWORD							m_dwCreateTime;
	DWORD							m_dwCreateDelay;
	INFO							m_tStandingInfo[3];
	RECT							m_tStandingRect[3];
};

#endif // !__STAGE1_BOSSTEXTBOX_H__


