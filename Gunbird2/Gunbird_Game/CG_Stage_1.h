// 기명준
// 게임 CG 씬
// 스테이지 클리어 후 엔딩에 해당하는 씬

#pragma once

#ifndef __CG_STAGE_1_H__
#define __CG_STAGE_1_H__

#include "Scene.h"
class CCG_Stage_1 :
	public CScene
{
public:
	CCG_Stage_1();
	virtual ~CCG_Stage_1();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Key_Check();

private:
	
};

#endif // !__CG_STAGE_1_H__


