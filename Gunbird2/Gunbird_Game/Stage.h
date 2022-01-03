// 기명준
// 게임 스테이지 씬
// 게임 플레이에 필요한 몬스터 생성 및 키 처리

#pragma once

#ifndef __STAGE_H__
#define __STAGE_H__

#include "Scene.h"
class CStage :
	public CScene
{
public:
	CStage();
	virtual ~CStage();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Load_Stage();
	void Key_Check();
	void Create_Object();
	void Stage_1();
	void Create_Object_Stage_1();

private:
	CObj*			m_pBackground;				// 스크롤되는 배경 포인터
	int				m_iStage;					// 스테이지 번호
	CREATE_OBJECT	m_arrCreateObjcet[300];		// 스테이지에 생성할 오브젝트 배열
};

#endif // !__STAGE_H__


