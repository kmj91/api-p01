// 기명준
// 로고 씬
// 게임의 제일 처음, 코인을 추가해서 게임 시작 가능

#pragma once

#ifndef __LOGO_H__
#define __LOGO_H__

#include "Scene.h"
class CLogo :
	public CScene
{
public:
	CLogo();
	virtual ~CLogo();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Key_Check();

private:
	
};

#endif // !__LOGO_H__


