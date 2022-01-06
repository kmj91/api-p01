// 기명준
// UI 고정
// 별다른 처리없이 초기화 받은 프레임키로 렌더만 함

#pragma once

#ifndef __UI_STATIC_H__
#define __UI_STATIC_H__

#include "Obj.h"
class CUI_Static final :
	public CObj
{
public:
	CUI_Static();
	virtual ~CUI_Static();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__UI_STATIC_H__


