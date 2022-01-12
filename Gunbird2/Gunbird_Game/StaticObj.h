// 기명준
// 고정 오브젝트
// 배경의 일부이거나 높이를 표현하기 위해 덮어씌우는 오브젝트

#pragma once

#ifndef __STATICOBJ_H__
#define __STATICOBJ_H__

#include "Obj.h"
class CStaticObj :
	public CObj
{
public:
	CStaticObj();
	virtual ~CStaticObj();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Update_RectEx();	// 이미지 RECT 정보 갱신
};

#endif // !__STATICOBJ_H__


