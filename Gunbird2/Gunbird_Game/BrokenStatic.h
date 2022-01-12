// 기명준
// 파괴되는 고정 오브젝트
// 배경의 일부이거나 높이를 표현하기 위해 덮어씌우는 오브젝트
// 파괴되기 때문에 CStaticObj 구분됨

#pragma once

#ifndef __BROKENSTATIC_H__
#define __BROKENSTATIC_H__

#include "Obj.h"
class CBrokenStatic :
	public CObj
{
public:
	CBrokenStatic();
	virtual ~CBrokenStatic();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Update_RectEx();	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
};

#endif // !__BROKENSTATIC_H__


