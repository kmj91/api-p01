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
	CObj*			m_pBackground;
	int				m_iStage;
	CREATE_OBJECT	m_arrCreateObjcet[300];
};

#endif // !__STAGE_H__


