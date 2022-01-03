// 기명준
// 캐릭터 선택 씬
// 캐릭터 선택 화면

#pragma once

#ifndef __SELECTMENU_H__
#define __SELECTMENU_H__

#include "Scene.h"
class CSelectMenu :
	public CScene
{
public:
	CSelectMenu();
	virtual ~CSelectMenu();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Key_Check();
	void Frame_Move();

private:
	CObj*		m_pSelectFocus;					// 플레이어 선택 오브젝트
	FRAME		m_tFrame[PLAYER::NAME_END];		// 플레이어 이미지 프레임 정보
};

#endif // !__SELECTMENU_H__


